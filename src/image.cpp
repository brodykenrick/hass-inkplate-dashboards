#include "homeplate.h"

bool remotePNG(const char *url)
{
    if (url == NULL) {
         Serial.print("[IMAGE] ERROR: got null image!");
         return false;
    }
    Serial.print("[IMAGE] Downloading image: ");
    Serial.println(url);

    displayStart();
    #if defined(ARDUINO_INKPLATE10) || defined(ARDUINO_INKPLATE10V2)
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
    vTaskDelay(0.25 * SECOND/portTICK_PERIOD_MS);

    displayEnd();
    i2cEnd();
    Serial.println("[IMAGE] displaying done.");
    return true;
}

static uint16_t _pngX = 0;
static uint16_t _pngY = 0;
