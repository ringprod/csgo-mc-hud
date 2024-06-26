#include "../include/raylib.h"
#include "render.h"
#include "../server/gamedata.h"
#include <stdio.h>

extern GameData gameData;

int hotbarX(int index, int sr, int hotbarWidth)
{
    return (GetScreenWidth() / sr / 2 + (index * 20 + 3) - hotbarWidth / 2) * sr;
}

void renderHotbar(int sr, long* updateCounter, long* healthUpdateCounter, long* lastSystemTime, int health, int* lastHealth, int* playerHealth, int foodLevel, int saturation, int armor, float xpProgress, int* offset, int* foodOffset, int* shouldPlaySound, Texture2D widgets, Texture2D icons, Texture2D bomb, Texture2D *crossbow, Texture2D *bow, Texture2D knife, Texture2D barrier)
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


    // xp progress
    if (xpProgress > 0.0f)
    {
        int length = (int)(xpProgress * 182.0f);
        int xpHeight = 5;
        Rectangle xpBar = { 0, 69, length, xpHeight };
        Rectangle xpDestRect = {
            centerX - hotbarWidth * sr / 2,
            bottomY - xpHeight * sr - (hotbarHeight + 2) * sr,
            length * sr,
            xpHeight * sr
        };
        DrawTexturePro(icons, xpBar, xpDestRect, origin, 0, WHITE);
    }

    // stats 
    int i = health;
    bool flag = *healthUpdateCounter > (long)*updateCounter && (*healthUpdateCounter - (long)*updateCounter) / 3L % 2L == 1L;

    //if (*lastHealth > health)
    if (i < *playerHealth)
    {
        *lastSystemTime = GetTime();
        *healthUpdateCounter = (long)(*updateCounter + 20);
        *shouldPlaySound = 1;
    }
    //else if (*lastHealth < health)
    else if (i > *playerHealth)
    {
        *lastSystemTime = GetTime();
        *healthUpdateCounter = (long)(*updateCounter + 10);
        *shouldPlaySound = 0;
    }

    if (GetTime() - *lastSystemTime > 1000L)
    {
        //*lastHealth = health;
        *playerHealth = i;
        *lastHealth = i;
        *lastSystemTime = GetTime();
        *shouldPlaySound = 0;
    }

    *playerHealth = i;

    int j = *lastHealth;
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

    // armor
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
        //int k5 = 16; //texture offset for heart

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

        //int i5 = 0;

        //printf("%d %d %d %d %d %d \n", k4, l4, 16 + i4 * 9, 9 * i5, 9, 9);
        {
            Rectangle heart = { 16 + i4 * 9, 0, 9, 9 };
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

    Weapons playerWeapons;
    memcpy(&playerWeapons, &gameData.player.weapons, sizeof(Weapons));

    printf("RENDER HAS DATA!!! :: COUNT: %d, FIRST NAME: %s\n", playerWeapons.count, playerWeapons.weaponArray[0].name);

    int slotOffset = 0;

    // knife
    for (int itemIndex = 0; itemIndex < playerWeapons.count; itemIndex++)
    {
        Weapon currentWeapon = playerWeapons.weaponArray[itemIndex];

        int hotbarY = (GetScreenHeight() / sr - (16 - 3) - 6) * sr + 1;

        if (strcmp(currentWeapon.type, "Knife") == 0)
        {
            printf("knife texture displayed\n");
            Rectangle knifeRect = { 0, 0, knife.width, knife.height };
            Rectangle knifeDestRect = {
                hotbarX(slotOffset, sr, hotbarWidth),
                hotbarY,
                16 * sr,
                16 * sr
            };
            DrawTexturePro(knife, knifeRect, knifeDestRect, origin, 0, WHITE);
            slotOffset++;
            break;
        }
        else
        {
            //printf("item index %d: no texture displayed %s\n", itemIndex, currentWeapon.type);
        }

    }

    // rilfe
    for (int itemIndex = 0; itemIndex < playerWeapons.count; itemIndex++)
    {
        Weapon currentWeapon = playerWeapons.weaponArray[itemIndex];

        int hotbarY = (GetScreenHeight() / sr - (16 - 3) - 6) * sr + 1;

        if (strcmp(currentWeapon.type, "Rifle") == 0 ||
            strcmp(currentWeapon.type, "Submachine Gun") == 0 ||
            strcmp(currentWeapon.type, "Machine Gun") == 0 ||
            strcmp(currentWeapon.type, "SniperRifle") == 0 ||
            strcmp(currentWeapon.type, "Shotgun") == 0)
        {
            printf("crossbow texture displayed\n");
            Rectangle crossbowRect = { 0, 0, crossbow[3].width, crossbow[3].height };
            Rectangle crossbowDestRect = {
                hotbarX(slotOffset, sr, hotbarWidth),
                hotbarY,
                16 * sr,
                16 * sr
            };
            DrawTexturePro(crossbow[3], crossbowRect, crossbowDestRect, origin, 0, WHITE);
            slotOffset++;
            break;
        }
        else
        {
            //printf("item index %d: no texture displayed %s\n", itemIndex, currentWeapon.type);
        }

    }

    // pistol
    for (int itemIndex = 0; itemIndex < playerWeapons.count; itemIndex++)
    {
        Weapon currentWeapon = playerWeapons.weaponArray[itemIndex];

        int hotbarY = (GetScreenHeight() / sr - (16 - 3) - 6) * sr + 1;

        if (strcmp(currentWeapon.type, "Pistol") == 0)
        {
            printf("bow texture displayed\n");
            Rectangle bowRect = { 0, 0, bow[3].width, bow[3].height };
            Rectangle bowDestRect = {
                hotbarX(slotOffset, sr, hotbarWidth),
                hotbarY,
                16 * sr,
                16 * sr
            };
            DrawTexturePro(bow[3], bowRect, bowDestRect, origin, 0, WHITE);
            slotOffset++;
            break;
        }
        else
        {
            //printf("item index %d: no texture displayed %s\n", itemIndex, currentWeapon.type);
        }

    }

    // bomb
    for (int itemIndex = 0; itemIndex < playerWeapons.count; itemIndex++)
    {
        Weapon currentWeapon = playerWeapons.weaponArray[itemIndex];

        int hotbarY = (GetScreenHeight() / sr - (16 - 3) - 6) * sr;

        if (strcmp(currentWeapon.type, "C4") == 0)
        {
            printf("bomb texture displayed\n");
            Rectangle bombRect = { 16, 1, bomb.width - 32, bomb.height - 2 };
            Rectangle bombDestRect = {
                (GetScreenWidth() / sr / 2 + (slotOffset * 20 + 4) - hotbarWidth / 2)* sr,
                hotbarY + 1 + (sr * 0.3),
                14 * sr,
                16 * sr - (sr * 0.6)
            };
            DrawTexturePro(bomb, bombRect, bombDestRect, origin, 0, WHITE);
            slotOffset++;
            break;
        }
        else
        {
            //printf("item index %d: no texture displayed %s\n", itemIndex, currentWeapon.type);
        }

    }


#if 0
    int slotOffset = 0;
    bool hasPrimaryWeapon = false;

    for (int itemIndex = 0; itemIndex < playerWeapons.count; itemIndex++)
    {
        Weapon currentWeapon = playerWeapons.weaponArray[itemIndex];

        int hotbarY = (GetScreenHeight() / sr - (16 - 3) - 6) * sr;

        if (strcmp(currentWeapon.type, "Knife") == 0)
        {
            printf("knife texture displayed\n");
            Rectangle knifeRect = { 0, 0, knife.width, knife.height };
            Rectangle knifeDestRect = {
                hotbarX(slotOffset, sr, hotbarWidth),
                hotbarY,
                14 * sr,
                16 * sr
            };
            DrawTexturePro(knife, knifeRect, knifeDestRect, origin, 0, WHITE);
            slotOffset++;
        }
        else if (strcmp(currentWeapon.type, "Rifle") == 0 ||
            strcmp(currentWeapon.type, "Submachine Gun") == 0 ||
            strcmp(currentWeapon.type, "Machine Gun") == 0 ||
            strcmp(currentWeapon.type, "SniperRifle") == 0 ||
            strcmp(currentWeapon.type, "Shotgun") == 0)
        {
            printf("crossbow texture displayed\n");
            Rectangle crossbowRect = { 0, 0, crossbow[4].width, crossbow[4].height};
            Rectangle crossbowDestRect = {
                hotbarX(slotOffset, sr, hotbarWidth),
                hotbarY,
                14 * sr,
                16 * sr
            };
            DrawTexturePro(crossbow[4], crossbowRect, crossbowDestRect, origin, 0, WHITE);
            slotOffset++;
        }
        else if (strcmp(currentWeapon.type, "Pistol") == 0)
        {
            printf("bow texture displayed\n");
            Rectangle bowRect = { 0, 0, bow[3].width, bow[3].height };
            Rectangle bowDestRect = {
                hotbarX(slotOffset, sr, hotbarWidth),
                hotbarY,
                14 * sr,
                16 * sr
            };
            DrawTexturePro(bow[3], bowRect, bowDestRect, origin, 0, WHITE);
            slotOffset++;
        }
        else
        {
            printf("item index %d: no texture displayed %s\n", itemIndex, currentWeapon.type);
        }

    }
#endif
}

//void renderHotbarItem(int hotbarX, int hotbarY, float elapsedFrameTime/*, EntityPlayer player, ItemStack stack*/)
//{
    // todo if i want to animatae the item coming into the inventory
//}

#if 0
// minecraft gui
private void renderSlot(PoseStack poseStack, int n, int n2, float f, Player player, ItemStack itemStack, int n3) {
    if (itemStack.isEmpty()) {
        return;
    }
    float f2 = (float)itemStack.getPopTime() - f;
    if (f2 > 0.0f) {
        float f3 = 1.0f + f2 / 5.0f;
        poseStack.pushPose();
        poseStack.translate(n + 8, n2 + 12, 0.0f);
        poseStack.scale(1.0f / f3, (f3 + 1.0f) / 2.0f, 1.0f);
        poseStack.translate(-(n + 8), -(n2 + 12), 0.0f);
    }
    this.itemRenderer.renderAndDecorateItem(poseStack, player, itemStack, n, n2, n3);
    if (f2 > 0.0f) {
        poseStack.popPose();
    }
    this.itemRenderer.renderGuiItemDecorations(poseStack, this.minecraft.font, itemStack, n, n2);
    }
#endif