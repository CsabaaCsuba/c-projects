echo off
set filename=%1
del %filename%.exe
REM glfw_3.4/lib_mingw64/libglfw3.a
g++ %filename%.cpp shaders/EBO.cpp shaders/shaderClass.cpp shaders/VAO.cpp shaders/VBO.cpp -Iglfw_3.4/include -Lglfw_3.4/lib_mingw64 -lglfw3 -Iglad/include -Lglad/src -lglad -lshell32 -lopengl32 -lglu32 -lgdi32 -o %filename%.exe
REM -Wall -Wextra -Werror
%filename%.exe

