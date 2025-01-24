gcc -c -Iglad/include -DBUILD_DLL glad/src/glad.c -o glad/src/glad.o
gcc -c -Iglad/include glad/src/glad.c -o glad/src/libgladll.o
ar rcs glad/src/libgladll.a glad/src/libgladll.o
gcc glad/src/glad.o -shared  -lshell32 -lopengl32 -lglu32 -lgdi32 -o glad/src/glad.dll

COPY ".\glad\src\glad.dll" glad.dll
COPY ".\glfw_3.4\lib_mingw64\glfw3.dll" glfw3.dll