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
-IE:\Tools\glad\include ^
-DUNITY_BUILD ^
-DSLOW ^
-DINTERNAL

set linker_args=user32.lib gdi32.lib winmm.lib shell32.lib opengl32.lib shcore.lib

set dll_pdb_file=game_%date:~-4,4%%date:~-10,2%%date:~-7,2%_%time:~0,2%%time:~3,2%%time:~6,2%.pdb

pushd src\shaders
for %%i in (./*) do glslangValidator %%i
popd

pushd build

del game_*.pdb > NUL 2> NUL

cl %compiler_args% -LD ..\src\game.cpp /link -incremental:no -PDB:%dll_pdb_file% && echo [32mGame DLL build successfull[0m || echo [31mGame DLL build failed[0m

cl %compiler_args% -Fe:cute ..\src\windows_main.cpp /link %linker_args% && echo [32mBuild successfull[0m || echo [31mBuild failed[0m

popd


:concat
set shaders=%shaders% %1
