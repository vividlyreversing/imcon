/*
 * Copyright (c) 2013 x8esix
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "imcon.h"

/// <summary>
///	Creates a console object and starts the child process </summary>
///
/// <param name="ac">
/// Uninitialized console object </param>
/// <param name="tzClientPath">
/// Path to client, in an inject environment this should not be relative </param>
///
/// <returns>
/// If child could be created </returns>
BOOL ImAllocConsole(OUT IM_CONSOLE* ac, IN const TCHAR* tzClientPath) {
       
    SecureZeroMemory(ac->tzClientPipe, MAX_NAME);

    // generate a random name (tmpnam has a forward slash and is not necessarily alphanumeric)
    for (register size_t i = 0; i < rand() % MAX_NAME; ++i) {
        ac->tzClientPipe[i] = rand() % 26 + 'A'; // make random letter
        ac->tzClientPipe[i] ^= rand() % 1 << 5;  // randomly toggle 0x20 (upper/lowercase)
    }
        
    { // create job object and set information
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION joELI;

        ac->hJob = CreateJobObject(NULL, ac->tzClientPipe);
        if (ac->hJob == NULL)
            return FALSE;

	    joELI.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
	    if(!SetInformationJobObject(ac->hJob, JobObjectExtendedLimitInformation, &joELI, sizeof(JOBOBJECT_EXTENDED_LIMIT_INFORMATION))) {
            CloseHandle(ac->hJob), ac->hJob = NULL;
            return FALSE;
        }
    }

    { // create child process
        SECURITY_ATTRIBUTES sa = {0};

        sa.nLength = sizeof(sa);
        sa.bInheritHandle = TRUE;
        //  sa.lpSecurityDescriptor = NULL;

        if (!CreatePipe(&ac->hRead, &ac->hWrite, &sa, BUFSIZ)) {
            CloseHandle(ac->hJob), ac->hJob = NULL;
            return FALSE;
        }

        STARTUPINFO siCon = {0};
        PROCESS_INFORMATION piCon = {0};

        siCon.cb = sizeof(siCon);
        siCon.hStdInput = ac->hRead;
        siCon.dwFlags |= STARTF_USESTDHANDLES;

        if (!CreateProcess(tzClientPath, NULL, NULL, NULL, TRUE, CREATE_NEW_CONSOLE | CREATE_BREAKAWAY_FROM_JOB | CREATE_UNICODE_ENVIRONMENT, NULL, NULL, &siCon, &piCon)) {
            CloseHandle(ac->hJob), ac->hJob = NULL;
            return FALSE;
        }

        DuplicateHandle(GetCurrentProcess(), piCon.hProcess, GetCurrentProcess(), &ac->hChildProcess, 0, FALSE, DUPLICATE_SAME_ACCESS);
        CloseHandle(piCon.hThread);
        CloseHandle(piCon.hProcess);
    }

    return AssignProcessToJobObject(ac->hJob, ac->hChildProcess);
}

/// <summary>
///	Frees a console and closes the job </summary>
///
/// <param name="ac">
/// Console object to free </param>
void ImFreeConsole(INOUT IM_CONSOLE* ac) {

    if (ac->hWrite != INVALID_HANDLE_VALUE && ac->hWrite != NULL)
        CloseHandle(ac->hWrite), ac->hWrite = NULL;
    if (ac->hRead != INVALID_HANDLE_VALUE && ac->hRead != NULL)
        CloseHandle(ac->hRead), ac->hRead == NULL;
    if (ac->hChildProcess != INVALID_HANDLE_VALUE && ac->hChildProcess != NULL) {
        TerminateProcess(ac->hChildProcess, 0xdead);
        CloseHandle(ac->hChildProcess);
    }

    TerminateJobObject(ac->hJob, 0xdead);
}

/// <summary>
///	Printf that specifies a different console output </summary>
///
/// <param name="ac">
/// Active console object </param>
/// <param name="format">
/// Format string </param>
///
/// <returns>
/// If string was written </returns>
BOOL Imprintf(IN const IM_CONSOLE* ac, IN const TCHAR* format, ...) {
	va_list vlist;
	TCHAR vargv[BUFSIZ] = {0};
	DWORD dwBytes;
	
	va_start(vlist, format);
	_vsntprintf(vargv, BUFSIZ, format, vlist);
	va_end(vlist);

	BOOL bResult = WriteFile(ac->hWrite, vargv, _tcslen(vargv), &dwBytes, NULL);
	return bResult && (dwBytes == _tcslen(vargv));
}
