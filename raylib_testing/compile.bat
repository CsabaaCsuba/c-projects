set filename=%1
echo %original_dir %
del %filename%.exe
gcc %filename%.c -IC:\raylib\raylib\src -LC:\raylib\raylib\src -lraylib -lopengl32 -lgdi32 -lwinmm -std=c99 -o %filename%.exe
REM -Wall -Wextra -Werror
%filename%.exe