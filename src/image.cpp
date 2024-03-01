#include "homeplate.h"

void displayStats()
{
    #if 0
    displayStart();
    display.setTextColor(C_BLACK, C_WHITE); // Set text color to black on white
    display.setFont(&Roboto_12);
    display.setTextSize(1);
    // display status message
    display.setCursor(1155, 820);

    // text to print over box
    display.printf("[%s]", timeString().c_str());
    displayEnd();
    #else
    //This is the text over, skip it to get things to render
    #endif
}

bool remotePNG(const char *url)
{
    if (url == NULL) {
         Serial.print("[IMAGE] ERROR: got null image!");
         return false;
    }
    displayStatusMessage("Downloading image...");
    Serial.print("[IMAGE] Downloading image: ");
    Serial.println(url);

    displayStart();
    #if 0 //Not a fn for inkplatecolor
    display.selectDisplayMode(INKPLATE_3BIT); // set grayscale mode
    #endif
    display.clearDisplay();                   // refresh the display buffer before rendering.
    displayEnd();


    display.println("drawImage from web from our HAss dash....");
    if (!display.drawImage(url, 0, 0, true, false))
    {
        // If is something failed (wrong filename or format), write error message on the screen.
        display.println("Image open error");
        display.display();
    }
   
    // check for stop - it could have happened inside drawImage()
    if (stopActivity())
    {
        displayStart();
        display.clearDisplay(); // refresh the display buffer before rendering.
        displayEnd();
        return false;
    }
    Serial.println("[IMAGE] displaying....");
    i2cStart();
    displayStart();
    display.display();
    // wait before releasing the i2c bus while the display settles. Helps prevent image fading
    #if 0
    vTaskDelay(0.25 * SECOND/portTICK_PERIOD_MS);
    #else
    vTaskDelay(1.0 * SECOND/portTICK_PERIOD_MS);
    #endif
    displayEnd();
    i2cEnd();
    Serial.println("[IMAGE] displaying done.");
    return true;
}

static uint16_t _pngX = 0;
static uint16_t _pngY = 0;


// returns height
// y value should be the top of the text location
uint16_t centerTextX(const char *t, int16_t x1, int16_t x2, int16_t y, bool lock)
{
    // center text
    int16_t x1b, y1b;
    uint16_t w, h;
    // y = n to give plenty of room for text to clear height of screen
    display.getTextBounds(t, 0, 100, &x1b, &y1b, &w, &h);

    int16_t x = ((x2 - x1) - w) / 2 + x1;

    if (lock)
        displayStart();
    display.setCursor(x, y + h);
    display.print(t);
    if (lock)
        displayEnd();
    return h;
}

// NOTE I2C & display locks MUST NOT be held by caller.
void displayStatusMessage(const char *format, ...)
{
    static char statusBuffer[100];
    // setup format string
    va_list argptr;
    va_start(argptr, format);
    vsnprintf(statusBuffer, 100, format, argptr);
    va_end(argptr);

    Serial.printf("[STATUS] %s\n", statusBuffer);

    #if 0

    i2cStart();
    displayStart();
    #if 0 //Not a fn for inkplatecolor
    display.selectDisplayMode(INKPLATE_1BIT);
    display.setTextColor(BLACK, WHITE);       // Set text color to black on white
    #else
    display.setTextColor(INKPLATE_RED, WHITE);       // Set text color on white
    #endif

    
    display.setFont(&Roboto_16);
    display.setTextSize(1);

    const int16_t pad = 3;           // padding
    const int16_t mar = 5;           // margin
    #if 0
    const int16_t statusWidth = 400; // extra space to clear for text
    #else
    const int16_t statusWidth = 50; // extra space to clear for text
    #endif
    const int16_t x = mar;
    const int16_t y = E_INK_HEIGHT - mar;

    // get text size for box
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(statusBuffer, x, y, &x1, &y1, &w, &h);

    // background box to set internal buffer colors
    display.fillRect(x - pad, y - pad - h, max(w + (pad * 2), statusWidth), h + (pad * 2), WHITE);
    //Serial.printf("fillRect(x:%u, y:%u, w:%u, h:%u)\n", x-pad, y-pad-h, max(w+(pad*2), 400), h+(pad*2));
    #if 0
    display.partialUpdate(sleepBoot);
    #else
    display.display();
    #endif

    // display status message
    display.setCursor(x, y);

    // text to print over box
    display.print(statusBuffer);
    #if 0
    display.partialUpdate(sleepBoot);
    #else
    display.display();
    #endif
    displayEnd();
    i2cEnd();

    #else
    //The color display is too slow to render to have on screen status messages -- just log to console for now
    //Perhaps log to mqtt later?
    return;
    #endif
}

void splashScreen()
{
    #if 0
    static const char *splashName = "HomePlate10";
    #else
    static const char *splashName = "HomePlate6";
    #endif
    displayStart();
    #if 0 //Not a fn for inkplatecolor
    display.selectDisplayMode(INKPLATE_1BIT);
    display.setTextColor(BLACK, WHITE);       // Set text color to black on white
    #else
    display.setTextColor(INKPLATE_GREEN, WHITE);       // Set text color to black on white
    #endif
    display.setFont(&Roboto_128);
    display.setTextSize(1);

    #if 0
    int16_t x = 285;
    int16_t y = 461;
    #else
    int16_t x = 85;
    int16_t y = 150;
    #endif
    bool dynamicPlacement = false;
    if (dynamicPlacement)
    {
        //get text size for box
        int16_t x1, y1;
        uint16_t w, h;
        display.getTextBounds(splashName, 100, 100, &x1, &y1, &w, &h);
        x = (E_INK_WIDTH - w) / 2;
        y = (E_INK_HEIGHT - h) / 2 + h;
        Serial.printf("SplashScreen location (%d, %d)\n", x, y);
    }
    display.setCursor(x, y);

    // text to print over box
    display.print(splashName);
    displayEnd();
    i2cStart();
    displayStart();
    #if 0
    display.partialUpdate(sleepBoot);
    #else
    display.display();
    #endif
    displayEnd();
    i2cEnd();
}
