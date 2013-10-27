/*	
	imcon client

	Example implementation of a client for imcon. Will wait for messages until a null byte is written to it
*/

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

#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>

#define BUFSIZE 512

int _tmain(int argc, TCHAR* argv[]) {
	HANDLE hStdin,
		   hOut;
	DWORD dwRead;
	BOOL fSuccess;
	char chBuf[BUFSIZE];

	hOut = CreateFile(TEXT("CONOUT$"), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hOut == INVALID_HANDLE_VALUE)
		return 1;

    hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
        return 1;

	while (TRUE) {
		fSuccess = ReadFile(hStdin, chBuf, BUFSIZE, &dwRead, NULL);
		if(!fSuccess || !dwRead) // break loop if null byte read or failed to read (pipe closed)
			break;
		WriteFile(hOut,chBuf,dwRead,&dwRead, NULL);
	}
    return 0;
}