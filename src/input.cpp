#include "homeplate.h"


#define INPUT_TASK_PRIORITY 10

// only supported on the Inkplate10 (not v2)
#if defined(ARDUINO_INKPLATE10)

#define INT_PAD1 (1 << (PAD1 - 8)) // 0x04
#define INT_PAD2 (1 << (PAD2 - 8)) // 0x08
#define INT_PAD3 (1 << (PAD3 - 8)) // 0x10

// read a byte from the expander
unsigned int readMCPRegister(const byte reg)
{
    Wire.beginTransmission(IO_INT_ADDR);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(IO_INT_ADDR, 1);
    return Wire.read();
}

// assume i2clock is already taken
// debounce touches that are too quick
bool checkPad(uint8_t pad)
{
    if (display.readTouchpad(pad))
    {
        vTaskDelay(250 / portTICK_PERIOD_MS);
        return display.readTouchpad(pad);
    }
    return false;
}

#endif // defined(ARDUINO_INKPLATE10)

void setupWakePins()
{
    pinMode(WAKE_BUTTON, INPUT_PULLUP);
}