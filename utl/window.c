#include "../include/raylib.h"
#include "window.h"
#include <stdio.h>

extern int g;

void* raylib(void *vargp)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - keyboard input");

    Vector2 ballPosition = { (float)screenWidth/2, (float)screenHeight/2 };
    SetTargetFPS(60);
	
	g = 0;
	
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (IsKeyDown(KEY_RIGHT)) ballPosition.x += 2.0f;
        if (IsKeyDown(KEY_LEFT)) ballPosition.x -= 2.0f;
        if (IsKeyDown(KEY_UP))
		{
			ballPosition.y -= 2.0f;
			//global++;
		}
        if (IsKeyDown(KEY_DOWN))
		{
			ballPosition.y += 2.0f;
			//global--;
		}

        BeginDrawing();

            ClearBackground(RAYWHITE);
			
			char str[50];
			sprintf(str, "%d", g);

            DrawText("move the ball with arrow keys", 10, 10, 20, DARKGRAY);
			DrawText(str, 50, 50, 20, LIGHTGRAY);

            DrawCircleV(ballPosition, 50, MAROON);

        EndDrawing();
    }
    CloseWindow();
	return NULL;
}