@echo off

rem Change these values to match your file name and output name
set source_file=main.c
set output_file=test.exe

rem Compile the source file
echo Compiling %source_file%...
gcc -lws2_32 -g main.c window.c defaults.c error.c header.c request.c response.c server.c -o test.exe -Wall -std=c99 -Wno-missing-braces -lraylib -L ../lib -lopengl32 -lgdi32 -lwinmm -lm -pthread -lws2_32

rem Check if the compilation is successful
if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b 1
)

rem If the compilation is successful, run the output file
echo Compilation successful!
echo Running %output_file%...
test.exe

exit /b 0
