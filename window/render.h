#include <math.h>
#include <string.h>

#include "../include/raylib.h"

#ifndef RENDER_H
#define RENDER_H

void renderHotbar(int sr, int* updateCounter, long* healthUpdateCounter, long* lastSystemTime, int health, int* lastHealth, int* playerHealth, int foodLevel, int saturation, int armor, float xpProgress, int* offset, int* foodOffset, int* shouldPlaySound, Texture2D widgets, Texture2D icons, Texture2D bomb);

void renderHotbarItem(int hotbarX, int hotbarY, float elapsedFrameTime/*, EntityPlayer player, ItemStack stack*/);

#endif // !RENDER_H
