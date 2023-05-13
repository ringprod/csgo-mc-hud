#include <stdio.h>
#include "include/raylib.h"


short GetAsyncKeyState(int vKey);

void getAsyncInput()
{
    if (GetAsyncKeyState(0x26) && 0x1)
    {
        printf("UP\n");
    }
    if (GetAsyncKeyState(0x28) && 0x1)
    {
        printf("DOWN\n");
    }
}

void renderHotbar(int sr, int health, float xp, Texture2D widgets, Texture2D icons)
{
    int centerX = GetScreenWidth() / 2;
    int bottomY = GetScreenHeight();
    Vector2 origin = { 0, 0 };

    // hotbar
    int hotbarWidth = 182;
    int hotbarHeight = 22;
    Rectangle hotbar = { 0, 0, hotbarWidth, hotbarHeight };
    Rectangle hotbarDestRect = {
        centerX - hotbarWidth * sr / 2,
        bottomY - hotbarHeight * sr,
        hotbarWidth * sr,
        hotbarHeight * sr
    };

    DrawTexturePro(widgets, hotbar, hotbarDestRect, origin, 0, WHITE);

    // xp bar

    int xpHeight = 5;
    Rectangle xpBar = { 0, 64, hotbarWidth, xpHeight };
    Rectangle xpDestRect = {
        centerX - hotbarWidth * sr / 2,
        bottomY - xpHeight * sr - (hotbarHeight + 2) * sr,
        hotbarWidth * sr,
        xpHeight* sr
    };

    DrawTexturePro(icons, xpBar, xpDestRect, origin, 0, WHITE);

    // hearts
    {
        int heartSize = 9;
        int heartX = (centerX - (hotbarWidth * sr) / 2);
        int heartY = (bottomY - xpHeight * sr - (hotbarHeight + 2) * sr) - (heartSize + 1) * sr;

        for (int i = 0; i < 10; i++)
        {
            Rectangle heart = { 16, 0, heartSize, heartSize };
            Rectangle heartDestRect = {
                heartX + i * (heartSize - 1) * sr,
                heartY,
                heartSize * sr,
                heartSize * sr
            };

            DrawTexturePro(icons, heart, heartDestRect, origin, 0, WHITE);
        }

        // draw full hearts
        int numFullHearts = (health / 10);
        for (int i = 0; i < numFullHearts; i++)
        {
            Rectangle heart = { 52, 0, heartSize, heartSize };
            Rectangle heartDestRect = {
                heartX + i * (heartSize - 1) * sr,
                heartY,
                heartSize * sr,
                heartSize * sr
            };

            DrawTexturePro(icons, heart, heartDestRect, origin, 0, WHITE);
        }
        // draw half hearts
        int numHalfHearts = health % 10 / 5;
        if (numHalfHearts > 0)
        {
            Rectangle heart = { 61, 0, heartSize, heartSize };
            Rectangle heartDestRect = {
                heartX + numFullHearts * (heartSize - 1) * sr,
                heartY,
                heartSize * sr,
                heartSize * sr
            };

            DrawTexturePro(icons, heart, heartDestRect, origin, 0, WHITE);
        }
    }

    // crosshair
    int crosshairSize = 15;
    Rectangle crosshair = { 0, 0, crosshairSize, crosshairSize };
    Rectangle crosshairDestRect = {
        centerX - crosshairSize * sr / 2, 
        bottomY / 2 - crosshairSize * sr / 2,
        crosshairSize * sr,
        crosshairSize * sr 
    };

    DrawTexturePro(icons, crosshair, crosshairDestRect, origin, 0, WHITE);
}

int main (void)
{
    int windowWidth = 1920;
    int windowHeight = 1080 + 1;

    int scaledResolution = 4;

    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_MOUSE_PASSTHROUGH | FLAG_WINDOW_TOPMOST);

    InitWindow(windowWidth, windowHeight, "raylib [core] example - basic window");
    SetTargetFPS(165);
    //SetExitKey(0);
    bool exitWindow = false;


    Texture2D widgets = LoadTexture("1.19.4/assets/minecraft/textures/gui/widgets.png");
    Texture2D icons = LoadTexture("1.19.4/assets/minecraft/textures/gui/icons.png");
    Texture2D background = LoadTexture("1.19.4/assets/minecraft/textures/gui/light_dirt_background.png");

    int health = 56;
    int lastHealth = health;
    float xp = 0.73;

    double currentTime = GetTime();
    double accumulator = 0.0;
    double timePerFrame = 1.0 / 20.0;
    //int counter = 0;

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
            bool flag = healthUpdateCounter > (long)updateCounter && (healthUpdateCounter - (long)updateCounter) / 3L % 2L == 1L;
            //printf("Hello, world! %d\n", counter++);
            if (lastHealth < health)
            {
                lastSystemTime = GetTime();
                healthUpdateCounter = (long)(updateCounter + 20);
            }
            else if (lastHealth > health)
            {
                lastSystemTime = GetTime();
                healthUpdateCounter = (long)(updateCounter + 10);
            }

            if (GetTime() - lastSystemTime > 1000L)
            {
                lastHealth = health;
                lastSystemTime = GetTime();
            }

            lastHealth = health;

            accumulator -= timePerFrame;
        }


        getAsyncInput();
        BeginDrawing();
        ClearBackground(BLANK);
        renderHotbar(scaledResolution, health, xp, widgets, icons);
        //DrawText("Congrats! You created your first window!", 0, 0, 20, LIGHTGRAY);
        //DrawTexturePro(icons, heart, destRect, origin, 0, WHITE);
        EndDrawing();
    }

    CloseWindow();

    UnloadTexture(widgets);
    UnloadTexture(icons);
    UnloadTexture(background);

    return 0;
}