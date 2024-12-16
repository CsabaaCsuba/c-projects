echo off
set filename=%1
del %filename%.exe
g++ %filename%.cpp -o %filename%.exe -Wall -Wextra -Werror
%filename%.exe