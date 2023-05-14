#include <stdio.h>
#include <math.h>
#include <time.h>

#include "include/raylib.h"


short GetAsyncKeyState(int vKey);

int health = 0;
int armor = 0;

bool isKeyPressed = false;

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

    if (GetAsyncKeyState('A') & 0x1)
    {
        if (!isKeyPressed)
        {
            if (armor < 20)
            {
                armor++;
            }
        }
        isKeyPressed = true;
    }
    else
    {
        isKeyPressed = false;
    }

    if (GetAsyncKeyState('D') & 0x1)
    {
        if (!isKeyPressed)
        {
            if (armor > 0)
            {
                armor--;
            }
        }
        isKeyPressed = true;
    }
    else
    {
        isKeyPressed = false;
    }

    if (GetAsyncKeyState('W') & 0x1)
    {
        if (!isKeyPressed)
        {
            if (health < 20)
            {
                health++;
            }
        }
        isKeyPressed = true;
    }
    else
    {
        isKeyPressed = false;
    }

    if (GetAsyncKeyState('S') & 0x1)
    {
        if (!isKeyPressed)
        {
            if (health > 0)
            {
                health--;
            }
        }
        isKeyPressed = true;
    }
    else
    {
        isKeyPressed = false;
    }
}

int offset[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void renderHotbar(int sr, int* updateCounter, long* healthUpdateCounter, long* lastSystemTime, int health, int* lastHealth, int armor, float xp, Texture2D widgets, Texture2D icons)
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
    {
        int xpHeight = 5;
        Rectangle xpBar = { 0, 64, hotbarWidth, xpHeight };
        Rectangle xpDestRect = {
            centerX - hotbarWidth * sr / 2,
            bottomY - xpHeight * sr - (hotbarHeight + 2) * sr,
            hotbarWidth * sr,
            xpHeight * sr
        };

        DrawTexturePro(icons, xpBar, xpDestRect, origin, 0, WHITE);
    }

    // armor
    bool flag = *healthUpdateCounter > (long)*updateCounter && (*healthUpdateCounter - (long)*updateCounter) / 3L % 2L == 1L;

    if (*lastHealth < health)
    {
        *lastSystemTime = GetTime();
        *healthUpdateCounter = (long)(*updateCounter + 20);
    }
    else if (*lastHealth > health)
    {
        *lastSystemTime = GetTime();
        *healthUpdateCounter = (long)(*updateCounter + 10);
    }

    if (GetTime() - *lastSystemTime > 1000L)
    {
        *lastHealth = health;
        *lastSystemTime = GetTime();
    }

    *lastHealth = health;

    int j = *lastHealth;
    int i = health;

    int l = (GetScreenWidth() / 2 - 91 * sr);
    int j1 = (GetScreenHeight() - 39 * sr);

    float f = 20.0f; // (float)iattributeinstance.getAttributeValue(); health?
    int k1 = 0.f; //MathHelper.ceil(entityplayer.getAbsorptionAmount()); absorption
    int l1 = ceil((f + (float)k1) / 2.0F / 10.0F);
    int i2 = fmax(10 - (l1 - 2), 3);
    int j2 = j1 - (l1 - 1) * i2 - 10 * sr;
    int l2 = k1;
    int i3 = armor; //entityplayer.getTotalArmorValue();
    int j3 = -1;


    for (int k3 = 0; k3 < 10; ++k3)
    {
        if (i3 > 0)
        {
            int l3 = l + k3 * 8 * sr;

            int armorSize = 9;

            if (k3 * 2 + 1 < i3)
            {
                //this.drawTexturedModalRect(l3, j2, 34, 9, 9, 9);
                Rectangle armor = { 34, 9, armorSize, armorSize };
                Rectangle armorDestRect = {
                    l3,
                    j2,
                    armorSize * sr,
                    armorSize * sr
                };
                DrawTexturePro(icons, armor, armorDestRect, origin, 0, WHITE);
            }

            if (k3 * 2 + 1 == i3)
            {
                //this.drawTexturedModalRect(l3, j2, 25, 9, 9, 9);
                Rectangle armor = { 25, 9, armorSize, armorSize };
                Rectangle armorDestRect = {
                    l3,
                    j2,
                    armorSize * sr,
                    armorSize * sr
                };
                DrawTexturePro(icons, armor, armorDestRect, origin, 0, WHITE);
            }

            if (k3 * 2 + 1 > i3)
            {
                //this.drawTexturedModalRect(l3, j2, 16, 9, 9, 9);
                
                Rectangle armor = { 16, 9, armorSize, armorSize };
                Rectangle armorDestRect = {
                    l3,
                    j2,
                    armorSize * sr,
                    armorSize * sr
                };
                DrawTexturePro(icons, armor, armorDestRect, origin, 0, WHITE);
            }
        }
    }

    //hearts

    for (int j5 = ceil((f + (float)k1) / 2.0F) - 1; j5 >= 0; --j5)
    {
        int k5 = 16; //texture offset for heart

        int i4 = 0;

        if (flag)
        {
            i4 = 1;
        }

        int j4 = ceil((float)(j5 + 1) / 10.0F) - 1;
        int k4 = l + (j5 % 10 * 8) * sr;
        int l4 = j1 - j4 * i2;

        if (i <= 4)
        {
            l4 += offset[j5];
        }

        if (l2 <= 0 && j5 == j3)
        {
            l4 -= 2;
        }

        int i5 = 0;

        //printf("%d %d %d %d %d %d \n", k4, l4, 16 + i4 * 9, 9 * i5, 9, 9);
        {
            Rectangle heart = { 16, 0, 9, 9 };
            Rectangle heartDestRect = {
                k4,
                l4,
                9 * sr,
                9 * sr
            };

            DrawTexturePro(icons, heart, heartDestRect, origin, 0, WHITE);
        }

        if (flag)
        {
            if (j5 * 2 + 1 < j)
            {
                // flashing full heart
                //printf("%d %d %d %d %d %d \n", k4, l4, k5 + 54, 9 * i5, 9, 9);
                Rectangle heart = { 70, 0, 9, 9 };
                Rectangle heartDestRect = {
                    k4,
                    l4,
                    9 * sr,
                    9 * sr
                };

                DrawTexturePro(icons, heart, heartDestRect, origin, 0, WHITE);
            }

            if (j5 * 2 + 1 == j)
            {
                // draw half flash heart
                //printf("%d %d %d %d %d %d \n", k4, l4, k5 + 63, 9 * i5, 9, 9);
                Rectangle heart = { 79, 0, 9, 9 };
                Rectangle heartDestRect = {
                    k4,
                    l4,
                    9 * sr,
                    9 * sr
                };

                DrawTexturePro(icons, heart, heartDestRect, origin, 0, WHITE);
            }
        }

        if (l2 > 0)
        {
            if (l2 == k1 && k1 % 2 == 1)
            {
                printf("%d %d %d %d %d %d \n", k4, l4, k5 + 153, 9 * i5, 9, 9);
                --l2;
            }
            else
            {
                printf("%d %d %d %d %d %d \n", k4, l4, k5 + 144, 9 * i5, 9, 9);
                l2 -= 2;
            }
        }

        // full hearts
        if (j5 * 2 + 1 < i)
        {
            Rectangle heart = { 52, 0, 9, 9 };
            Rectangle heartDestRect = {
                k4,
                l4,
                9 * sr,
                9 * sr
            };

            DrawTexturePro(icons, heart, heartDestRect, origin, 0, WHITE);
            //printf("%d %d %d %d %d %d \n", k4, l4, k5 + 36, 9 * i5, 9, 9);
        }
        // half hearts
        if (j5 * 2 + 1 == i)
        {
            Rectangle heart = { 61, 0, 9, 9 };
            Rectangle heartDestRect = {
                k4,
                l4,
                9 * sr,
                9 * sr
            };
            DrawTexturePro(icons, heart, heartDestRect, origin, 0, WHITE);
            //printf("%d %d %d %d %d %d \n", k4, l4, k5 + 45, 9 * i5, 9, 9);
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

int main(void)
{
    time_t t;
    srand((unsigned)time(&t));

    int windowWidth = 1920;
    int windowHeight = 1080 + 1;

    int scaledResolution = 4;

    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_MOUSE_PASSTHROUGH | FLAG_WINDOW_TOPMOST);

    InitWindow(windowWidth, windowHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);
    //SetExitKey(0);
    bool exitWindow = false;


    Texture2D widgets = LoadTexture("1.19.4/assets/minecraft/textures/gui/widgets.png");
    Texture2D icons = LoadTexture("1.19.4/assets/minecraft/textures/gui/icons.png");
    Texture2D background = LoadTexture("1.19.4/assets/minecraft/textures/gui/light_dirt_background.png");

    //int health = 3;
    int lastHealth = health;
    float xp = 0.73;
    //int armor = 8;

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
            //printf("Hello, world! %d\n", counter++);
            for (int count = 0; count < 10; count++) {
                offset[count] = rand() % 2 * scaledResolution;
            }

            accumulator -= timePerFrame;
        }

        getAsyncInput();
        BeginDrawing();
        ClearBackground(BLANK);

        renderHotbar(scaledResolution, &updateCounter, &healthUpdateCounter, &lastSystemTime, health, &lastHealth, armor, xp, widgets, icons);
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