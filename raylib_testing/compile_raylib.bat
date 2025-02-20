set /p PLATFORM_TYPE=Enter platform type (default: DESKTOP): 
if "%PLATFORM_TYPE%"=="" set PLATFORM_TYPE=DESKTOP

set PLATFORM_VAR=PLATFORM_%PLATFORM_TYPE%

set original_dir=%cd%
cd C:\raylib\raylib\src
mingw32-make PLATFORM=%PLATFORM_VAR%
cd %original_dir%
