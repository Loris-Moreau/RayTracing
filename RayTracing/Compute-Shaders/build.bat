@echo off

:: Adjust the path below to your Visual Studio install location 
::call "E:\Files\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64_x86

REM — Make sure vswhere.exe is on your PATH (or use its full path):
set VSWHERE="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

REM — Enable delayed expansion so we can see InstallDir inside the loop
setlocal EnableDelayedExpansion

REM — Find the latest VS install that has the VC toolset
for /f "usebackq tokens=*" %%i in (`
  %VSWHERE% -latest -products * ^
    -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 ^
    -property installationPath
`) do (
  set InstallDir=%%i
)

REM — Use delayed expansion here (!) and CALL to run the batch
if exist "!InstallDir!\VC\Auxiliary\Build\vcvarsall.bat" (
  call "!InstallDir!\VC\Auxiliary\Build\vcvarsall.bat" amd64_x86 %*
) else (
  echo ERROR: Could not find vcvarsall.bat in "!InstallDir!" & exit /b 1
)


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
