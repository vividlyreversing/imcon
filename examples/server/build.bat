:: build script for hello world imcon server

gcc -c server.c -Os -s -std=c99 -g
gcc -o server.exe server.o ..\..\Release\imcon.lib -Os -s
del server.o

pause