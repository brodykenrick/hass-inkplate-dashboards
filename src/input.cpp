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

void checkBootPads()
{
    #if defined(ARDUINO_INKPLATE10)
        unsigned int key = 0;
        i2cStart();
        key = readMCPRegister(MCP23017_INTFB);
        i2cEnd();
        if (key) // which pin caused interrupt
        {
            // Serial.printf("INTFB: %#x\n", keyInt);
            //  value of pin at time of interrupt
            if (key & INT_PAD1)
            {
                Serial.println("[INPUT] boot: PAD1");
                startActivity(HomeAssistant);
            }

            Serial.println();

            i2cStart();
            key = readMCPRegister(MCP23017_INTCAPB); // this clears the interrupt
            i2cEnd();
            // Serial.printf("INTCAP: %#x\n", keyValue);
            //  if (keyValue & INT_PAD1)
            //  {
            //      Serial.print("PAD1 ");
            //  }
            //  if (keyValue & INT_PAD2)
            //  {
            //      Serial.print("PAD2 ");
            //  }
            //  if (keyValue & INT_PAD3)
            //  {
            //      Serial.print("PAD3 ");
            //  }
            //  Serial.println();
        }
    #endif
}

void setupWakePins()
{
    #if defined(ARDUINO_INKPLATE10)
        // set which pads can allow wakeup
        display.setIntPin(PAD1, RISING, IO_INT_ADDR);
        display.setIntPin(PAD2, RISING, IO_INT_ADDR);
        display.setIntPin(PAD3, RISING, IO_INT_ADDR);
    #endif
    pinMode(WAKE_BUTTON, INPUT_PULLUP);

}