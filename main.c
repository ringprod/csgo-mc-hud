#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "include/raylib.h"


short GetAsyncKeyState(int vKey);

int health = 20;
int food = 20;
int armor = 13;
int saturation = 1;
float xpLevel = 0.7f;

bool isKeyPressed = false;

int partialTicks = 0;

int debugX = 0, debugY = 0;

void getAsyncInput()
{
    if (GetAsyncKeyState('A') & 0x1)
    {
        if (!isKeyPressed)
        {
            //debugX--;
            //printf("x: %d y: %d\n", debugX, debugY);
            if (xpLevel > 0.0f)
                xpLevel -= 0.1f;
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
            //debugX++;
            //printf("x: %d y: %d\n", debugX, debugY);
            if (xpLevel < 1.0f)
                xpLevel += 0.1f;
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
            debugY--;
            printf("x: %d y: %d\n", debugX, debugY);
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
            debugY++;
            printf("x: %d y: %d\n", debugX, debugY);
        }
        isKeyPressed = true;
    }
    else
    {
        isKeyPressed = false;
    }
}

int offset[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int foodOffset[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void renderHotbar(int sr, int* updateCounter, long* healthUpdateCounter, long* lastSystemTime, int health, int* lastHealth, int foodLevel, int armor, Texture2D widgets, Texture2D icons)
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


    // xp levels
    if (xpLevel > 0.0f)
    {
        int length = (int)(xpLevel * 182.0f);
        int xpHeight = 5;
        Rectangle xpBar = { 0, 69, length, xpHeight };
        Rectangle xpDestRect = {
            centerX - hotbarWidth * sr / 2,
            bottomY - xpHeight * sr - (hotbarHeight + 2) * sr,
            length* sr,
            xpHeight * sr
        };
        DrawTexturePro(icons, xpBar, xpDestRect, origin, 0, WHITE);

        char s[20];
        //sprintf(s, "%d", xpLevel);
        int i1 = (GetScreenWidth() / sr - MeasureText("23", 64) / sr) / 2;
        int j1 = GetScreenHeight() / sr - 31 - 4;
        DrawText("23", (i1 + 1) * sr, j1 * sr, 64, LIGHTGRAY);
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
    int k = foodLevel;

    int l = (GetScreenWidth() / 2 - 91 * sr);
    int i1 = GetScreenWidth() / 2 + 91 * sr;
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
        /* absorption logic
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
        }*/

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

    // food

    for (int l5 = 0; l5 < 10; ++l5)
    {
        int j6 = j1;
        int l6 = 16;
        int j7 = 0;

        if (saturation == 0 && *updateCounter % (k * 3 + 1) == 0)
        //if (k < 6)
        {
            j6 = foodOffset[l5];
        }

        int l7 = i1 - l5 * 8 * sr - 9 * sr;
        //this.drawTexturedModalRect(l7, j6, 16 + j7 * 9, 27, 9, 9);
        {
            Rectangle food = { 16 + j7, 27, 9, 9 };
            Rectangle foodDestRect = {
                l7,
                j6,
                9 * sr,
                9 * sr
            };
            DrawTexturePro(icons, food, foodDestRect, origin, 0, WHITE);
        }

        if (l5 * 2 + 1 < k)
        {
            //this.drawTexturedModalRect(l7, j6, l6 + 36, 27, 9, 9);
            {
                Rectangle food = { l6 + 36, 27, 9, 9 };
                Rectangle foodDestRect = {
                    l7,
                    j6,
                    9 * sr,
                    9 * sr
                };
                DrawTexturePro(icons, food, foodDestRect, origin, 0, WHITE);
            }
        }

        if (l5 * 2 + 1 == k)
        {
            //this.drawTexturedModalRect(l7, j6, l6 + 45, 27, 9, 9);
            {
                Rectangle food = { l6 + 45, 27, 9, 9 };
                Rectangle foodDestRect = {
                    l7,
                    j6,
                    9 * sr,
                    9 * sr
                };
                DrawTexturePro(icons, food, foodDestRect, origin, 0, WHITE);
            }
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

    // hotbar items
    for (int l = 0; l < 9; ++l)
    {
        int i1 = (GetScreenWidth() / sr / 2 + (l * 20 + 3) - hotbarWidth/2) * sr;
        int j1 = (GetScreenHeight() / sr - (16 - 3) - 5) * sr;
        //renderHotbarItem(i1, j1, partialTicks/*, entityplayer, entityplayer.inventory.mainInventory.get(l)*/ );
        Rectangle item = { 16, 54, 16, 16 };
        Rectangle itemDestRect = {
            i1,
            j1,
            16 * sr,
            16 * sr
        };
        DrawTexturePro(icons, item, itemDestRect, origin, 0, WHITE);
    }
}

void renderHotbarItem(int hotbarX, int hotbarY, float elapsedFrameTime/*, EntityPlayer player, ItemStack stack*/)
{
    // todo
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

    Texture2D widgets = LoadTexture("res/1.19.4/assets/minecraft/textures/gui/widgets.png");
    Texture2D icons = LoadTexture("res/1.19.4/assets/minecraft/textures/gui/icons.png");
    Texture2D background = LoadTexture("res/1.19.4/assets/minecraft/textures/gui/light_dirt_background.png");
    Font font = LoadFontEx("res/minecraft-font/MinecraftRegular-Bmg3.otf", 96, 0, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);

    //int health = 3;
    int lastHealth = health;
    //float xp = 0.73;
    //int armor = 8;

    // intialize shake heart movement
    for (int count = 0; count < 10; count++) {
        offset[count] = rand() % 2 * scaledResolution;
    }

    // intialize shake hunger bar movement
    for (int count = 0; count < 10; count++) {
        foodOffset[count] = (GetScreenHeight() - 39 * scaledResolution) + rand() % 3 - 1;
    }

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
            updateCounter++;
            //printf("Hello, world! %d\n", counter++);
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

        getAsyncInput();
        BeginDrawing();
        ClearBackground(BLANK);

        renderHotbar(scaledResolution, &updateCounter, &healthUpdateCounter, &lastSystemTime, health, &lastHealth, food, armor, widgets, icons);
        //DrawText("Congrats! You created your first window!", 0, 0, 20, LIGHTGRAY);
        //DrawTexturePro(icons, heart, destRect, origin, 0, WHITE);
       
        char charList[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
        DrawTextEx(font, charList, (Vector2) { 20.0f, 100.0f }, 500, 1, WHITE);

        EndDrawing();
    }

    CloseWindow();

    UnloadTexture(widgets);
    UnloadTexture(icons);
    UnloadTexture(background);
    UnloadFont(font);

    return 0;
}