echo off
set filename=%1
del %filename%.exe
gcc %filename%.c -o %filename%.exe
%filename%.exe