#include "include/raylib.h"
#include "font.h"

void readFontTexture(Texture2D texture, int* charWidths)
{
    const int charWidth = 8;
    const int charHeight = 8;
    const int spacing = 0;

    const int charsPerRow = 16;
    const int charsPerCol = 16;

    bool isTransparent = true;

    Image image = LoadImageFromTexture(texture);
    Color* colors = LoadImageColors(image);

    //char charList[] = "                                 !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

    for (int characterRow = 0; characterRow < charsPerRow; characterRow++)
    {
        for (int characterCol = 0; characterCol < charsPerCol; characterCol++)
        {
            int index = characterRow * charsPerCol + characterCol;
            int width = 0;

            for (int px = 7; px >= 0; px--)
            {
                for (int py = 7; py >= 0; py--)
                {
                    int pixelX = characterCol * 8 + px;
                    int pixelY = characterRow * 8 + py;
                    //printf("%c", colors[pixelY * 128 + pixelX].a == 255 ? '*' : ' ');
                    if (colors[pixelY * 128 + pixelX].a == 255)
                    {
                        width = px + 1;
                        isTransparent = false;
                    }
                    if (!isTransparent)
                        break;
                }
                //printf("\n");

                if (!isTransparent)
                    break;
            }
            //printf("%d width of %d\n", charList[characterRow * charsPerCol + characterCol], width);
            if (characterRow * charsPerCol + characterCol == 32)
                charWidths[characterRow * charsPerCol + characterCol] = 4;
            else
                charWidths[characterRow * charsPerCol + characterCol] = width;

            isTransparent = true;
            //printf("\n%d ^------------\n", width);
        }

    }

    charWidths[34] = 4;

    UnloadImage(image);
}

void drawMCText(Texture2D font, const char* str, int x, int y, float scale, int spacing, Color color, Rectangle* chars, int* charWidths) {
    scale = scale * 0.8;
    char charList[] = "×××××××××××××××××××××××××××××××× !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    for (int i = 0; i < strlen(str); i++) {
        const char* ptr = strchr(charList, str[i]);
        if (ptr) {
            int charIndex = ptr - charList;
            float charScale = scale * charWidths[charIndex] / 8;
            chars[charIndex].width = charWidths[charIndex];
            Rectangle charRect = chars[charIndex];
            Rectangle destRect = (Rectangle){ x, y, charWidths[charIndex] * scale, 8 * scale };
            //Vector2 origin = (Vector2){ charRect.width / 2, charRect.height / 2 };
            Vector2 origin = (Vector2){ 0, 0 };
            DrawTexturePro(font, charRect, destRect, origin, 0, color);
            x += (charWidths[charIndex] + spacing) * scale;
            //printf("%d: char %c width: %d\n", i, charList[charIndex], charWidths[charIndex]);
        }
    }
}

int getMCTextWidth(const char* str, int scale, Texture2D font, int* charWidths) {
    int totalWidth = 0;
    char charList[] = "×××××××××××××××××××××××××××××××× !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    for (int i = 0; i < strlen(str); i++) {
        const char* ptr = strchr(charList, str[i]);
        if (ptr) {
            int charIndex = ptr - charList;
            totalWidth += charWidths[charIndex] + 1;
        }
    }
    return totalWidth == 0 ? 0 : (totalWidth - 1) * scale;
}
