@echo off

:: Adjust the path below to your Visual Studio install
call "E:\Files\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64_x86


set compilerFlags=/Zi /fp:fast /arch:AVX2 -DPLATFORM_WIN32=1 -DWIN32_GPU

IF "%~1"=="-d" (
    :: Debug build
    set compilerFlags=/Od -DSINGLE_THREAD=1 %compilerFlags%
) ELSE (
    set compilerFlags=/O2 %compilerFlags%
)

pushd ..\bin\
cl %compilerFlags% ..\Compute-Shaders\main_win32.cpp ..\Compute-Shaders\glad.c ..\Compute-Shaders\glad_wgl.c /link user32.lib opengl32.lib Gdi32.lib Kernel32.lib
IF %ERRORLEVEL%==0 (
    .\main_win32.exe
)
popd
