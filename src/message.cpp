#include "homeplate.h"

// message buffer
static char message[MESSAGE_BUFFER_SIZE];

void setMessage(const char *m)
{
    strlcpy(message, m, MESSAGE_BUFFER_SIZE);
}

const char* getMessage()
{
    return message;
}

static const GFXfont *fonts[] = {&Roboto_128, &Roboto_64, &Roboto_32, &Roboto_16, &Roboto_12};

struct FontSizing
{
    const GFXfont *font;
    uint16_t height;
    uint16_t width;
    uint8_t lineHeight;
    uint8_t yAdvance;
};

FontSizing findFontSizeFit(char *m)
{
    int16_t x1, y1;
    FontSizing font;

    // display.setTextWrap(false);

    for (size_t i = 0; i < sizeof(fonts) / sizeof(fonts[0]); i++)
    {
        font.font = fonts[i];
        display.setFont(font.font);
        // y = n to give plenty of room for text to clear height of screen
        display.getTextBounds(m, 0, (uint8_t)font.font->yAdvance, &x1, &y1, &font.width, &font.height);
        font.yAdvance = (uint8_t)font.font->yAdvance;
        font.lineHeight = font.height % font.yAdvance;
        // Serial.printf("[MESSAGE][DEBUG] Testing font # %u with height = %u and width = %u  bounds = (%d, %d) lineHeight = %d\n", i, font.height, font.width, x1, y1, font.lineHeight);
        if (font.width <= E_INK_WIDTH && font.height <= E_INK_HEIGHT)
        {
            // Serial.printf("[MESSAGE] Using font %u with height = %u and width = %u  bounds = (%d, %d) screen(%d, %d) lineHeight = %d\n", i, font.height, font.width, x1, y1, E_INK_WIDTH, E_INK_HEIGHT, font.lineHeight);
            return font;
        }
    }
    Serial.printf("[MESSAGE][ERROR] Unable to find good font size for %s\n", m);
    return font;
}

