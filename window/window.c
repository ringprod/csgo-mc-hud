#include "../include/raylib.h"
#include "window.h"
#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <time.h>
#include <string.h>

#include "../server/gamedata.h"

#include "render.h"
#include "font.h"

short GetAsyncKeyState(int vKey);

extern GameData gameData;

void getAsyncInput(bool* isKeyPressed, int* debugX, int* debugY, float* var)
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
            //*debugY -= 1;
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
            //*debugY += 1;
            printf("x: %d y: %d\n", *debugX, *debugY);
        }
        *isKeyPressed = true;
    }
    else
    {
        *isKeyPressed = false;
    }
}

void* raylib(void* vargp)
{
    int debugX = 0, debugY = 0;
    bool xEditMode = false;
    bool yEditMode = false;

    time_t t;
    srand((unsigned)time(&t));

    int windowWidth = 1920;
    int windowHeight = 1080 + 1;

    //int windowWidth = 854;
    //int windowHeight = 480;

    Rectangle chars[128];
    int charWidths[256];

    int offset[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int foodOffset[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    int scaledResolution = 4;

    int health = 20;
    int food = 20;
    int armor = 0;
    int saturation = 1;
    float xpProgress = 0.0f;
    int xpLevel = 0;

    bool isKeyPressed = false;

    int partialTicks = 0;

    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_MOUSE_PASSTHROUGH | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_RESIZABLE);
    
    InitAudioDevice();

    InitWindow(windowWidth, windowHeight, "csgo mc hud");
    SetTargetFPS(60);

    //SetExitKey(0);
    bool exitWindow = false;

    Texture2D widgets = LoadTexture("res/1.19.4/assets/minecraft/textures/gui/widgets.png");
    Texture2D icons = LoadTexture("res/1.19.4/assets/minecraft/textures/gui/icons.png");
    Texture2D background = LoadTexture("res/1.19.4/assets/minecraft/textures/gui/light_dirt_background.png");
    Texture2D font = LoadTexture("res/1.19.4/assets/minecraft/textures/font/ascii.png");

    // items

    Texture2D crossbow[5] = { 0 };
    crossbow[0] = LoadTexture("res/1.19.4/assets/minecraft/textures/item/crossbow_pulling_0.png");        //crossbow_0
    crossbow[1] = LoadTexture("res/1.19.4/assets/minecraft/textures/item/crossbow_pulling_1.png");        //crossbow_1
    crossbow[2] = LoadTexture("res/1.19.4/assets/minecraft/textures/item/crossbow_pulling_2.png");        //crossbow_2
    crossbow[3] = LoadTexture("res/1.19.4/assets/minecraft/textures/item/crossbow_standby.png");          //crossbow_standby
    crossbow[4] = LoadTexture("res/1.19.4/assets/minecraft/textures/item/crossbow_arrow.png");            //crossbow_loaded 

    Texture2D bow[4] = { 0 };
    bow[0] = LoadTexture("res/1.19.4/assets/minecraft/textures/item/bow_pulling_0.png");
    bow[1] = LoadTexture("res/1.19.4/assets/minecraft/textures/item/bow_pulling_1.png");
    bow[2] = LoadTexture("res/1.19.4/assets/minecraft/textures/item/bow_pulling_2.png");
    bow[3] = LoadTexture("res/1.19.4/assets/minecraft/textures/item/bow.png");
    
    Texture2D knife = LoadTexture("res/1.19.4/assets/minecraft/textures/item/iron_sword.png");
    Texture2D defuser = LoadTexture("res/1.19.4/assets/minecraft/textures/item/shears.png");
    Texture2D bomb = LoadTexture("res/tnt.png");

    Texture2D smoke = LoadTexture("res/1.19.4/assets/minecraft/textures/item/bone_meal.png");
    Texture2D decoy = LoadTexture("res/1.19.4/assets/minecraft/textures/item/light_gray_candle.png");
    // might use sunflower or glowstone dust for flash
    Texture2D flash = LoadTexture("res/1.19.4/assets/minecraft/textures/item/firework_rocket.png");
    Texture2D molotov = LoadTexture("res/1.19.4/assets/minecraft/textures/item/blaze_powder.png");
    Texture2D hegrenade = LoadTexture("res/1.19.4/assets/minecraft/textures/item/fire_charge.png");
    Texture2D taser = LoadTexture("res/1.19.4/assets/minecraft/textures/item/redstone.png");

    Sound hit[3] = { 0 };
    hit[0] = LoadSound("res/sounds/hit1.ogg");
    hit[1] = LoadSound("res/sounds/hit2.ogg");
    hit[2] = LoadSound("res/sounds/hit3.ogg");

    int playerHealth = health;
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

    long updateCounter;
    long healthUpdateCounter;
    long lastSystemTime;

    int shouldPlaySound = 0;

    while (!exitWindow)
    {
        if (WindowShouldClose()) exitWindow = true;

        double newTime = GetTime();
        double deltaTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += deltaTime;

        health = gameData.player.state.health / 5;
        armor = gameData.player.state.armor / 5;
        xpLevel = gameData.player.match_stats.kills;

        if (gameData.player.state.health > 0 && health == 0)
            health = 1;
        if (gameData.player.state.armor > 0 && armor == 0)
            armor = 1;

        while (accumulator >= timePerFrame)
        {
            updateCounter++;
            if (health <= 4)
            {
                for (int count = 0; count < 10; count++) {
                    offset[count] = rand() % 2 * scaledResolution;
                }
            }

            // simplifed saturation, may impliment logic later
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
        if (gameData.map.phase != NULL && !strcmp(gameData.map.phase, "live"))
        {
            if (shouldPlaySound)
            {
                float randomFloat = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
                int randomSound = rand() % 3;
                SetSoundPitch(hit[randomSound], randomFloat * 0.2f + 1.0f);
                PlaySound(hit[randomSound]);
                shouldPlaySound = 0;
            }
            renderHotbar(scaledResolution, &updateCounter, &healthUpdateCounter, &lastSystemTime, health, &lastHealth, &playerHealth, food, saturation, armor, xpProgress, offset, foodOffset, &shouldPlaySound, widgets, icons, bomb);
        }

        if (gameData.map.phase != NULL && xpLevel > 0 && !strcmp(gameData.map.phase, "live"))
        {
            char charList[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
            char s[20];
            int textSize = 5;

            sprintf(s, "%d", xpLevel);

            int centerWidth = GetScreenWidth() / 2;
            int hotbarHeight = (GetScreenHeight() / scaledResolution - (31 + 4)) * scaledResolution;

            int offset = (strlen(s) == 1) ? 3 : strlen(s) * 4;
            drawOutlinedMCText(font, s, centerWidth - (getMCTextWidth(s, textSize, font, charWidths) / 2 * scaledResolution) - 1 * scaledResolution, hotbarHeight, textSize, 1, YELLOW, chars, charWidths, scaledResolution);
        }
        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();

    UnloadTexture(widgets);
    UnloadTexture(icons);
    UnloadTexture(background);
    UnloadTexture(font);

    for (int i = 0; i < 5; i++) UnloadTexture(crossbow[i]);

    for (int i = 0; i < 4; i++) UnloadTexture(bow[i]);

    UnloadTexture(knife);
    UnloadTexture(defuser);
    UnloadTexture(bomb);

    UnloadTexture(smoke);
    UnloadTexture(decoy);
    UnloadTexture(flash);
    UnloadTexture(molotov);
    UnloadTexture(hegrenade);

    UnloadTexture(taser);

    for (int i = 0; i < 3; i++) UnloadSound(hit[i]);

    return NULL;
}