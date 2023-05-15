#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "include/raylib.h"

#include "render.h"
#include "font.h"

short GetAsyncKeyState(int vKey);

void getAsyncInput(bool* isKeyPressed, int *debugX, int *debugY, float *var)
{
    if (GetAsyncKeyState('A') & 0x1)
    {
        if (!*isKeyPressed)
        {
            //debugX--;
            //printf("x: %d y: %d\n", debugX, debugY);
            if (*var > 0.0f)
                *var -= 0.1f;
        }
        *isKeyPressed = true;
    }
    else
    {
        *isKeyPressed = false;
    }

    if (GetAsyncKeyState('D') & 0x1)
    {
        if (!*isKeyPressed)
        {
            //debugX++;
            //printf("x: %d y: %d\n", debugX, debugY);
            if (*var < 1.0f)
                *var += 0.1f;
        }
        *isKeyPressed = true;
    }
    else
    {
        *isKeyPressed = false;
    }

    if (GetAsyncKeyState('W') & 0x1)
    {
        if (!*isKeyPressed)
        {
            *debugY--;
            printf("x: %d y: %d\n", *debugX, *debugY);
        }
        *isKeyPressed = true;
    }
    else
    {
        *isKeyPressed = false;
    }

    if (GetAsyncKeyState('S') & 0x1)
    {
        if (!*isKeyPressed)
        {
            *debugY++;
            printf("x: %d y: %d\n", *debugX, *debugY);
        }
        *isKeyPressed = true;
    }
    else
    {
        *isKeyPressed = false;
    }
}

int main(void)
{
    time_t t;
    srand((unsigned)time(&t));

    int windowWidth = 1920;
    int windowHeight = 1080 + 1;

    Rectangle chars[128];
    int charWidths[256];

    int offset[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int foodOffset[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    int scaledResolution = 4;

    int health = 4;
    int food = 0;
    int armor = 13;
    int saturation = 1;
    float xpProgress = 0.7f;
    int xpLevel = 1;

    bool isKeyPressed = false;

    int partialTicks = 0;

    int debugX = 0, debugY = 0;

    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_MOUSE_PASSTHROUGH | FLAG_WINDOW_TOPMOST);

    InitWindow(windowWidth, windowHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);
    //SetExitKey(0);
    bool exitWindow = false;

    Texture2D widgets = LoadTexture("res/1.19.4/assets/minecraft/textures/gui/widgets.png");
    Texture2D icons = LoadTexture("res/1.19.4/assets/minecraft/textures/gui/icons.png");
    Texture2D background = LoadTexture("res/1.19.4/assets/minecraft/textures/gui/light_dirt_background.png");
    Texture2D font = LoadTexture("res/1.19.4/assets/minecraft/textures/font/ascii.png");

    int lastHealth = health;

    // intialize shake heart movement
    for (int count = 0; count < 10; count++) {
        offset[count] = rand() % 2 * scaledResolution;
    }

    // intialize shake hunger bar movement
    for (int count = 0; count < 10; count++) {
        foodOffset[count] = (GetScreenHeight() - 39 * scaledResolution) + rand() % 3 - 1;
    }

    readFontTexture(font, charWidths);

    // mc font atlas
    int spacing = 0;
    for (int i = 0; i < 128; i++) {
        chars[i].x = (i % 16) * (8 + spacing);
        chars[i].y = (i / 16) * (8 + spacing);
        chars[i].width = charWidths[i];
        chars[i].height = 8;
    }

    double currentTime = GetTime();
    double accumulator = 0.0;
    double timePerFrame = 1.0 / 20.0;

    double startTime = GetTime();

    int updateCounter;
    long healthUpdateCounter;
    long lastSystemTime;

    while (!exitWindow)
    {
        if ((GetAsyncKeyState(0x1B) && 0x1) || WindowShouldClose()) exitWindow = true;

        double newTime = GetTime();
        double deltaTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += deltaTime;

        while (accumulator >= timePerFrame)
        {
            updateCounter++;
            if (health <= 4)
            {
                for (int count = 0; count < 10; count++) {
                    offset[count] = rand() % 2 * scaledResolution;
                }
            }

            // just for right now to make saturation simpler
            if (food <= 10)
            {
                saturation = 0;
            }
            else
            {
                saturation = 1;
            }

            if (saturation == 0 && updateCounter % (food * 3 + 1) == 0)
            {
                for (int count = 0; count < 10; count++) {
                    foodOffset[count] = (GetScreenHeight() - 39 * scaledResolution) + rand() % 3 - 1;
                }
            }

            accumulator -= timePerFrame;
        }

        getAsyncInput(&isKeyPressed, &debugX, &debugY, &xpProgress);
        BeginDrawing();
        ClearBackground(BLANK);

        //DrawText("Congrats! You created your first window!", 0, 0, 20, LIGHTGRAY);
        renderHotbar(scaledResolution, &updateCounter, &healthUpdateCounter, &lastSystemTime, health, &lastHealth, food, saturation, armor, xpProgress, offset, foodOffset, widgets, icons);

        char charList[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
        //drawMCText(font, "test among us amongy YOU!Yessir! fortnite", 10, 10, 5, 1, YELLOW);

        //char s[20];
        int textSize = 5;
        //sprintf(s, "%d", xpProgress);
        //int i1 = (GetScreenWidth() / scaledResolution + getMCTextWidth("32", textSize, font) / scaledResolution) / 2;
        int i1 = (GetScreenWidth() / scaledResolution) / 2;
        int j1 = GetScreenHeight() / scaledResolution - 31 - 4;
        drawMCText(font, "1", (i1 + debugY) * scaledResolution, j1 * scaledResolution + 1, textSize, 1, YELLOW, chars, charWidths);
        drawMCText(font, "43", 0, 0, 5, 1, YELLOW, chars, charWidths);
        //DrawFPS(10, 10);


        EndDrawing();
    }

    CloseWindow();

    UnloadTexture(widgets);
    UnloadTexture(icons);
    UnloadTexture(background);
    UnloadTexture(font);

    return 0;
}