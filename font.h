#include "include/raylib.h"

#ifndef FONT_H
#define FONT_H

void readFontTexture(Texture2D texture, int* charWidths);

void drawMCText(Texture2D font, const char* str, int x, int y, float scale, int spacing, Color color, Rectangle* chars, int* charWidths, int sr);

void drawOutlinedMCText(Texture2D font, const char* str, int x, int y, float scale, int spacing, Color color, Rectangle* chars, int* charWidths, int sr);

int getMCTextWidth(const char* str, int scale, Texture2D font, int* charWidths);

#endif