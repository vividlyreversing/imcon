:: Windows build script for imcon
:: x8esix

@echo off

set objects=

:: cleanup     
     FOR %%i in (*.o) DO del %%i
     IF EXIST ..\Release\apcon.lib del ..\Release\apcon.lib /Q

cd .\imcon\
     FOR %%i in (*.c) DO (gcc -c %%i -std=c99 -Os -s & ECHO Compiling %%i)

     ECHO.
     ECHO Linking library...
     ECHO.
     FOR %%i in (*.o) DO (call :concat %%i & ECHO Adding %%i to library)

     ar rcs ..\Release\imcon.lib %objects%
ECHO.
ECHO Cleaning up...
ECHO.
    
     FOR %%i in (*.o) DO (del %%i & ECHO Deleting %%i...)

ECHO.
ECHO Compilation complete!
ECHO.
pause

:concat:
set objects=%objects% %1
goto :eof