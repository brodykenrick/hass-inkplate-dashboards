#include "homeplate.h"

uint getBatteryPercent(double voltage)
{
    if (voltage < BATTERY_VOLTAGE_LOW)
        return 0;

    uint percentage = ((voltage - BATTERY_VOLTAGE_LOW) * 100.0) / (BATTERY_VOLTAGE_HIGH - BATTERY_VOLTAGE_LOW);
    if (percentage > 100)
        percentage = 100;
    if (percentage < 0)
        percentage = 0;
    return percentage;
}

const char *bootReason()
{
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch (wakeup_reason)
    {
    case ESP_SLEEP_WAKEUP_EXT0:
        return "wake button"; // (EXT0_RTC_IO) is the wake button...
    case ESP_SLEEP_WAKEUP_EXT1:
        return "touchpads"; // (EXT1_RTC_CNTL)
    case ESP_SLEEP_WAKEUP_TIMER:
        return "timer";
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
        return "internal touchpad";
    case ESP_SLEEP_WAKEUP_ULP:
        return "ULP program";
    default:
        return "normal";
    }
}

void printChipInfo()
{
    /* Print chip information */
    Serial.printf("ESP Chip information:\n");
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    Serial.printf("This is %s chip with %u CPU core(s), WiFi%s%s, ",
                  CONFIG_IDF_TARGET,
                  chip_info.cores,
                  (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
                  (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    Serial.printf("silicon revision %u, ", chip_info.revision);

    Serial.printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
                  (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    Serial.printf("Minimum free heap size: %u bytes\n", esp_get_minimum_free_heap_size());

    heap_caps_print_heap_info(MALLOC_CAP_32BIT | MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM | MALLOC_CAP_INTERNAL);
}

/**
 * \brief Called if stack overflow during execution
 */
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask,
                                          signed char *pcTaskName)
{
    Serial.printf("\n\n!!! [DEBUG]: stack overflow %x %s\n\n", (unsigned int)pxTask, (portCHAR *)pcTaskName);
    /* If the parameters have been corrupted then inspect pxCurrentTCB to
     * identify which task has overflowed its stack.
     */
    for (;;)
    {
    }
}

void lowBatteryCheck()
{
    // need min uptime for voltage to be stable
    if (millis() < 2 * SECOND)
    {
        return;
    }
    double voltage = 0;
    #if !defined(ARDUINO_INKPLATE2)
    i2cStart();
    voltage = display.readBattery();
    i2cEnd();
    #endif

    // if voltage was < 1, it was a bad reading.
    if (voltage > 1 && voltage <= BATTERY_VOLTAGE_WARNING_SLEEP)
    {
        Serial.printf("[MAIN] voltage %.2f <= min %.2f, powering down\n", voltage, BATTERY_VOLTAGE_WARNING_SLEEP);
        // TODO mqtt send low battery sleep notification
        setSleepDuration(0xFFFFFFFF);
        gotoSleepNow();
    }
}

void printDebugStackSpace()
{
    if (DEBUG_STACK)
    {
        UBaseType_t uxHighWaterMark;
        uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        Serial.printf("[DEBUG][HEAP] TASK: %s; stack min remaining(%d) txPortGetFreeHeapSize(%d) xPortGetMinimumEverFreeHeapSize(%d) bytes\n", pcTaskGetTaskName(NULL), uxHighWaterMark, xPortGetFreeHeapSize(), xPortGetMinimumEverFreeHeapSize());
    }
}

// vTaskGetRunTimeStats and vTaskList are disabled in esp-idf :()
// void taskinfo()
// {
//     Serial.printf("Task info:\n");
//     auto num = uxTaskGetNumberOfTasks();
//     Serial.printf("total tasks: %d\n", num);
//     char buff[40 * num + 40]; // 40 bytes per task + 40
//     vTaskGetRunTimeStats(buff);
//     Serial.printf("vTaskGetRunTimeStats:\n%s", buff);
//     vTaskList(buff);
//     Serial.printf("vTaskList:\n%s", buff);
//     //uxTaskGetSystemState();
// }


void printDebug(const char *s)
{
    if (DEBUG_PRINT)
    {
        Serial.printf("[DEBUG]%s\n", s);
    }
}