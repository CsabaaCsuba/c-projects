echo off
set filename=%1
del %filename%.exe
gcc %filename%.c -o %filename%.exe
REM -Wall -Wextra -Werror
%filename%.exe