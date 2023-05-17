@echo off

rem Change these values to match your file name and output name
set source_file=main.c
set output_file=hud.exe

rem Compile the source file
echo Compiling %source_file%...
gcc -lws2_32 main.c include/cJSON.c window/render.c window/font.c window/window.c server/defaults.c server/error.c server/header.c server/request.c server/response.c server/server.c -o hud.exe -Wall -std=c99 -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm -lm -pthread -lws2_32

rem Check if the compilation is successful
if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b 1
)

rem If the compilation is successful, run the output file
echo Compilation successful!
echo Running %output_file%...
hud.exe

exit /b 0
