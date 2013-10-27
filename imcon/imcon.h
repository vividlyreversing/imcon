/*
	imcon

	Multiple console i/o host for windows xp+ using anonymous pipes
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

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <tchar.h>
#include <Windows.h>

#define IN
#define OUT
#define INOUT

#pragma region Options

#   define MAX_NAME MAX_PATH

#pragma endregion

#pragma region Structs

    typedef struct IM_VIRTUAL_CONSOLE {
	    HANDLE hWrite,
		       hRead,
		       hChildProcess;
        HANDLE hJob;
        TCHAR tzClientPipe[MAX_NAME];
    } IM_CONSOLE;

#pragma endregion

#pragma region Prototypes
    BOOL ImAllocConsole(OUT IM_CONSOLE* ac, IN const TCHAR* tzClientPath);
    void ImFreeConsole(INOUT IM_CONSOLE* ac);

    BOOL Imprintf(IN const IM_CONSOLE* ac, IN const TCHAR* format, ...);
#pragma endregion