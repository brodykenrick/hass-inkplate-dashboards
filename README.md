# HomePlate (stripped back to just be a dashboard displayer and working with inkplate6color and inkplate10)

[Home Assistant](https://www.home-assistant.io/) E-Ink Dashboard on the [Inkplate 10](https://www.crowdsupply.com/e-radionica/inkplate-10)

![homeplate](https://user-images.githubusercontent.com/164192/150657050-d919b70e-d3a0-42e3-a842-9d7684b0dcc7.jpeg)

## [Activities Screenshots](activities.md)

## Features

* Display Home Assistant dashboards on a beautiful e-ink display
* Makes full use of the ESP32's cores with [FreeRTOS](https://www.freertos.org/)
* Reports sensor data to Home Assistant over MQTT (Temperature, Battery, WiFi, etc..)
* Can change Activity displayed via MQTT Command (HASS dashboard, Image/Dashboard, etc..)
* Syncs RTC over NTP
* OTA updates over WiFi
* Power saving sleep mode.
* Display any PNG image (via URL) from MQTT Command

## Future Ideas

* Incorporate [WiFi Manager](https://github.com/tzapu/WiFiManager) for settings

## Setup

### Home Assistant Dashboard

Create a Home Assistant Dashboard you want to display. I recommend using the [kiosk-mode](https://github.com/maykar/kiosk-mode), [card-mod](https://github.com/thomasloven/lovelace-card-mod) and [layout-card](https://github.com/thomasloven/lovelace-layout-card) plugins to customize and tune the dashboard for your display.

Setup [hass-lovelace-dashboard-inkplate-screenshotter](hhttps://github.com/brodykenrick/hass-lovelace-dashboard-inkplate-screenshotter) to regularly screenshot the desired dashboards for the InkPlate.

### More information in [hass.md](hass.md)

### Inkplate

Install [PlatformIO](https://platformio.org/). Then copy `src/config_example.h` to `src/config.h` and enter your settings.

Build & run with:

```shell
pio run
```

If you have the Inkplate10v2 (without the additional MCP expander and touchpads), use the `inkplate10v2` environment:

```shell
pio run -e inkplate10v2
```

TBD - Using for an inkplate6color details

The first flash/installation needs to be done over USB. Future updates can be done over USB or WiFi with:

```shell
pio run -e ota
```

To monitor serial output without re-flashing:

```shell
pio device monitor
```

### Updating

```shell
git pull
pio upgrade
pio pkg update
pio run --target clean
```

### Debugging

#### Touchpad Sensitivity

On some devices, the touchpads can be overly sensitive. This can cause lots of phantom touch events preventing the Homeplate from going into sleep and using up a lot of power.

Sometimes running `pio run --target=clean` can resolve this before you build & flash the firmware.

The touchpad sensitivity is set in hardware by resistors, but the touch sensors are calibrated on bootup when the Device first gets power. I have found that USB power can mess with this calibration. If you are using battery power, restarting the Homeplate (by using the power switch on the side of the PCB) without USB power attached is enough to fix the sensitivity.

Alternatively, the touchpads can be completely disabled by setting `#define TOUCHPAD_ENABLE false` in `config.h`.

#### Waveform

If you get the following error while booting your inkplate, run the [Inkplate_Wavefrom_EEPROM_Programming](https://github.com/e-radionicacom/Inkplate-Arduino-library/tree/master/examples/Inkplate10/Others/Inkplate_Wavefrom_EEPROM_Programming) example to update your Inkplate's waveform.

```text
Waveform load failed! Upload new waveform in EEPROM. Using default waveform.
```

Older Inkplates don't appear to ship with an updated waveform. I found waveform 3 looks the best for mine.
