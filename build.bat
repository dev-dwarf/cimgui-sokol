@echo off
IF NOT EXIST build mkdir build

pushd build

set SOURCES= ..\cimgui\cimgui.cpp ..\cimgui\imgui\imgui*.cpp ..\sokol\sokol.c
set INCLUDES=/I..\cimgui\ /I..\cimgui\imgui /I..\sokol /I..\include /I..\..
set LIBS=
set NOWARN=-wd4201 -wd4100 -wd4189 -wd4244 -wd4456 -wd4457 -wd4245
set FLAGS=/nologo /utf-8 /W4 /Oi /GR- /FC %NOWARN%
set DEF=/D IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1 /D IMGUI_IMPL_API=extern\t\"C\"\t__declspec\(dllexport\) /D ImTextureID=ImU64 /D UNICODE /D _UNICODE

IF "%1"=="full" cl /c %FLAGS% %INCLUDES% %DEF% %SOURCES% /link %LIBS%

rm -f demo.obj
cl %FLAGS% %INCLUDES% %DEF% ..\demo.c /link /NOEXP /NOIMPLIB %LIBS% *.obj /out:demo.exe

popd
