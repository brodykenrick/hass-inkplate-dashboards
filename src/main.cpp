#if !defined(ARDUINO_INKPLATE10) && !defined(ARDUINO_INKPLATE5) && !defined(ARDUINO_INKPLATE10V2) && !defined(ARDUINO_INKPLATECOLOR) && !defined(ARDUINO_INKPLATE2)
#error "No supported board defined, please select Inkplate 2, 5, 6color, 10 or 10v2."
#endif

#if ((defined(ARDUINO_INKPLATECOLOR) + defined(ARDUINO_INKPLATE10) + defined(ARDUINO_INKPLATE5) + defined(ARDUINO_INKPLATE10V2) + defined(ARDUINO_INKPLATE2)) > 1)
#error "Multiple boards defined - You can only define one board."
#endif

#include <driver/rtc_io.h> //ESP32 library used for deep sleep and RTC wake up pins
#include <rom/rtc.h>       // Include ESP32 library for RTC (needed for rtc_get_reset_reason() function)
#include "homeplate.h"

#if defined(ARDUINO_INKPLATE10) || defined(ARDUINO_INKPLATE10V10) || defined(ARDUINO_INKPLATE5)
Inkplate display(INKPLATE_1BIT);
#else
Inkplate display;
#endif
SemaphoreHandle_t mutexI2C, mutexSPI, mutexDisplay;

bool sleepBoot = false;

// Store int in rtc data, to remain persistent during deep sleep, reset on power up.
RTC_DATA_ATTR uint bootCount = 0;

void setup()
{
    Serial.begin(115200);
    Serial.printf("\n\n[SETUP] starting, version(%s) boot: %u\n", VERSION, bootCount);
    ++bootCount;
    // reset GPIOs used for wake interrupt
    rtc_gpio_deinit(GPIO_NUM_34);
    rtc_gpio_deinit(WAKE_BUTTON);

    // start inkplate display mutexes
    mutexI2C = xSemaphoreCreateMutex();
    mutexSPI = xSemaphoreCreateMutex();
    mutexDisplay = xSemaphoreCreateMutex();

    sleepBoot = (rtc_get_reset_reason(0) == DEEPSLEEP_RESET); // test for deep sleep wake

    // only run on fresh boot
    if (!sleepBoot)
        printChipInfo();

    // must be called before checkPads() so buttons can override pre-boot activity
    startActivity(DEFAULT_ACTIVITY);


    // Take the mutex
    displayStart();
    i2cStart();
    display.begin();             // Init Inkplate library (you should call this function ONLY ONCE)
    #if !defined(ARDUINO_INKPLATE2)
    //Note: Inkplate 2 doesn’t have dedicated RTC IC, but it has RTC built-in inside ESP32 that is not as precise as dedicated RTC IC, but it can be used for timekeeping, just time needs to be refreshed (updated) at least once a day using WiFi and NTP. Also, can’t keep time, when there is no power (doesn’t have RTC backup battery).
    //https://github.com/SolderedElectronics/Inkplate-Arduino-library/blob/master/examples/Inkplate2/Advanced/DeepSleep/Inkplate2_RTC_Alarm_With_Deep_Sleep/Inkplate2_RTC_Alarm_With_Deep_Sleep.ino#L76
    display.rtcClearAlarmFlag(); // Clear alarm flag from any previous alarm
    #endif
    setupWakePins();

    //TODO: Can this be removed?
    // setup display
    if (sleepBoot)
    {
        #if defined(ARDUINO_INKPLATE10) || defined(ARDUINO_INKPLATE10V2)
        display.preloadScreen(); // copy saved screen state to buffer
        #endif
    }
    display.clearDisplay();      // Clear frame buffer of display
    i2cEnd();
    displayEnd();

    #if !defined(ARDUINO_INKPLATE2)
    // print battery state
    double voltage = 0;
    i2cStart();
    voltage = display.readBattery();
    i2cEnd();
    voltage = roundf(voltage * 100) / 100; // rounds to 2 decimal places
    int percent = getBatteryPercent(voltage);
    Serial.printf("[SETUP] Battery: %d%% (%.2fv)\n", percent, voltage);
    #else
    // Can we get any battery or volateg info out of inkplate2 ??
    #endif

    Serial.println("[SETUP] starting time task");
    setupTimeAndSyncTask();

    Serial.println("[SETUP] starting WiFi task");
    wifiConnectTask();

    Serial.println("[SETUP] starting OTA task");
    startOTATask();

    Serial.println("[SETUP] starting MQTT task");
    startMQTTTask();

    Serial.println("[SETUP] starting sleep task");
    sleepTask();

    Serial.println("[SETUP] starting activities task");
    startActivitiesTask();
}

// if there is too much in here it can cause phantom touches...
void loop()
{
    // main loop runs at priority 1
    printDebug("[MAIN] loop...");
    vTaskDelay(3 * SECOND / portTICK_PERIOD_MS);
    lowBatteryCheck();
}
