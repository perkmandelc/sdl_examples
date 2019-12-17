@echo off

pushd build
copy ..\libs\SDL2-2.0.10-VC\lib\x64\SDL2.dll

cl -MTd -nologo -Gm- -GR- -EHa- -Od -Oi -FC -Z7 ../src/window_opengl.c -I../libs/SDL2-2.0.10-VC/include -link -incremental:no -opt:ref ../libs/SDL2-2.0.10-VC/lib/x64/SDL2.lib ../libs/SDL2-2.0.10-VC/lib/x64/SDL2main.lib opengl32.lib -SUBSYSTEM:CONSOLE
cl -MTd -nologo -Gm- -GR- -EHa- -Od -Oi -FC -Z7 ../src/input_output.c -I../libs/SDL2-2.0.10-VC/include -link -incremental:no -opt:ref ../libs/SDL2-2.0.10-VC/lib/x64/SDL2.lib ../libs/SDL2-2.0.10-VC/lib/x64/SDL2main.lib opengl32.lib -SUBSYSTEM:CONSOLE
cl -MTd -nologo -Gm- -GR- -EHa- -Od -Oi -FC -Z7 ../src/sound.c -I../libs/SDL2-2.0.10-VC/include -link -incremental:no -opt:ref ../libs/SDL2-2.0.10-VC/lib/x64/SDL2.lib ../libs/SDL2-2.0.10-VC/lib/x64/SDL2main.lib opengl32.lib -SUBSYSTEM:CONSOLE
popd
exit 0
