#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define RAYGUI_IMPLEMENTATION
#include "include/raylib.h"
#include "include/raygui.h"

#include "render.h"
#include "font.h"

int main(void)
{
    int debugX = 0, debugY = 0;
    bool xEditMode = false;
    bool yEditMode = false;
    
    time_t t;
    srand((unsigned)time(&t));

    //int windowWidth = 1920;
    //int windowHeight = 1080 + 1;
    
    int windowWidth = 854;
    int windowHeight = 480;
    
    Rectangle chars[128];
    int charWidths[256];

    int offset[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int foodOffset[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    int scaledResolution = 1 + debugX;

    int health = 4;
    int food = 0;
    int armor = 13;
    int saturation = 1;
    float xpProgress = 0.7f;
    int xpLevel = 1;

    bool isKeyPressed = false;

    int partialTicks = 0;

    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | /*FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_MOUSE_PASSTHROUGH |*/ FLAG_WINDOW_TOPMOST | FLAG_WINDOW_RESIZABLE);

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
        if (WindowShouldClose()) exitWindow = true;

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

        BeginDrawing();
        ClearBackground(BLANK);
        
        if (GuiValueBox((Rectangle){60, 100, 80, 25 }, "Scale", &debugX, 1, 20, xEditMode)) xEditMode = !xEditMode;
        scaledResolution = debugX;
        if (GuiValueBox((Rectangle){60, 150, 80, 25 }, "xp", &xpLevel, 1, 20, yEditMode)) yEditMode = !yEditMode;

        //debugX = GuiSlider((Rectangle) { 355, 400, 165, 20 }, "TEST", TextFormat("%2.2f", (float)debugX), debugX, -50, 100);
        //debugY = GuiSliderBar((Rectangle) { 320, 430, 200, 20 }, NULL, TextFormat("%i", (int)debugY), debugY, 0, 100);
        
        //DrawText("Congrats! You created your first window!", 0, 0, 20, LIGHTGRAY);
        renderHotbar(scaledResolution, &updateCounter, &healthUpdateCounter, &lastSystemTime, health, &lastHealth, food, saturation, armor, xpProgress, offset, foodOffset, widgets, icons);

        char charList[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
        char s[20];
        int textSize = 5;
        //int xp = 9 + debugY;
        sprintf(s, "%d", xpLevel);
        //int i1 = (GetScreenWidth() / scaledResolution + getMCTextWidth("32", textSize, font) / scaledResolution) / 2;
        int centerWidth = GetScreenWidth() / 2;
        int hotbarHeight = (GetScreenHeight() / scaledResolution - (31 + 4)) * scaledResolution;
        //printf("%d ", getMCTextWidth(s, textSize, font, charWidths));
        int offset = (strlen(s) == 1) ? 3 : strlen(s) * 4;

        //drawOutlinedMCText(font, s, (i1 - getMCTextWidth(s, textSize, font, charWidths) / 2) * scaledResolution-1 * 4 / scaledResolution, (j1 + 1) * scaledResolution + 1, textSize, 1, YELLOW, chars, charWidths, scaledResolution);
        printf("%d\n", getMCTextWidth(s, textSize, font, charWidths));
        drawOutlinedMCText(font, s, centerWidth - (getMCTextWidth(s, textSize, font, charWidths) / 2 * scaledResolution) - 1 * scaledResolution, hotbarHeight, textSize, 1, YELLOW, chars, charWidths, scaledResolution);
        
        //drawMCText(font, s, (i1 - getMCTextWidth(s, textSize, font, charWidths) / 2) * scaledResolution-1, (j1 + 1) * scaledResolution + 1, textSize, 1, YELLOW, chars, charWidths, scaledResolution);
        drawMCText(font, "43", 0, 30, 5, 1, YELLOW, chars, charWidths, scaledResolution);


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