@echo off

if not exist build mkdir build
if not exist engine mkdir engine

pushd engine
if not exist shaders mkdir shaders
popd

set compiler_args=^
-MTd ^
-nologo ^
-GR- ^
-EHa ^
-Od -Oi ^
-WX -W4 ^
-FC ^
-Zi ^
-diagnostics:caret ^
-wd4201 ^
-wd4100 ^
-wd4505 ^
-wd4189 ^
-wd4146 ^
-wd4127 ^
-wd4312 ^
-wd4239 ^
-Fe:cute ^
-IE:\Tools\glfw\include\GLFW ^
-IE:\Tools\glad\include ^
-DUNITY_BUILD

set linker_args=^
E:\Tools\glfw\build\src\Debug\glfw3.lib ^
user32.lib gdi32.lib shell32.lib opengl32.lib

pushd src\shaders
for %%i in (./*) do glslangValidator %%i
popd

pushd build

cl %compiler_args% ..\src\main.cpp /link %linker_args% && echo [32mBuild successfull[0m || echo [31mBuild failed[0m

popd


:concat
set shaders=%shaders% %1
