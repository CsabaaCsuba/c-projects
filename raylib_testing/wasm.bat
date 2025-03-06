@echo off
setlocal enabledelayedexpansion

set input=%1

:: Find last occurrence of /
set "folder=%input%"
set "filename=%input%"

:loop
if "!folder:~-1!"=="/" (
    set "folder=!folder:~0,-1!"
    set /a len+=1
    set "filename=!filename:~%len%!"
    goto end
)
if "!folder:~-1!"=="" (
    set "folder=."
    goto end
)
set /a len+=1
set "folder=!folder:~0,-1!"
goto loop

:end
echo Folder: %folder%
echo Filename: %filename%
echo Assets: %folder%/%folder%_assets

::emscripten needs at least one file in the assets folder.
set "assets_folder=%folder%/%folder%_assets"
if not exist "%assets_folder%" (
    echo Assets folder does not exist. Creating a message file...
    mkdir "%assets_folder%"
    echo "You must have at least one file in this directory" > "%assets_folder%/message.txt"
) else (
    echo Assets folder exists. Checking if it is empty...
    dir /b "%assets_folder%" | findstr . > nul
    if errorlevel 1 (
        echo Assets folder is empty. Creating a message file...
        echo "You must have at least one file in this directory" > "%assets_folder%/message.txt"
    )
)

emcc -o %folder%/%filename%_c.html %folder%/%filename%.c -Wall -std=c99 -sMEMORY64=0 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -Os -I. -I C:/raylib/raylib/src -I C:/raylib/raylib/src/external -L. -L C:/raylib/raylib/src -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=67108864 --preload-file %assets_folder% -s FORCE_FILESYSTEM=1 --shell-file ./shell.html C:/raylib/raylib/src/web/libraylib.a -DPLATFORM_WEB -s EXPORTED_RUNTIME_METHODS=ccall && timeout 0 && python -m http.server -d ./%folder%