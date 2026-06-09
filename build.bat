@echo off
echo Setting up MSVC environment...
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
echo Compiling 2D Graphics Editor...
cl.exe /W4 /O2 /std:c11 /D_CRT_SECURE_NO_WARNINGS /Fe:editor.exe main.c canvas.c object.c ui.c
if %errorlevel% neq 0 (
    echo Compilation failed!
    exit /b %errorlevel%
)
echo Compilation successful! Editor executable created as editor.exe
