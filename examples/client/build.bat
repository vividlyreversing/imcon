:: build script for hello world client

gcc -c client.c -Os -s -std=c99 -g
gcc -o client.exe client.o -Os -s
del client.o

pause