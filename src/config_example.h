#pragma once

// WiFi SSID
#define WIFI_SSID "WiFi Network Name" 
// WiFi password
#define WIFI_PASSWORD "WiFi Password"

// Hostname
// NOTE: if using multiple homeplate devices of the same type/size, then you MUST make the hostname unique
//#define HOSTNAME "homeplate_custom" // Set a hostname if you don't want a default "sized" name
#if !defined(HOSTNAME)
#if defined(ARDUINO_INKPLATE2)
#define HOSTNAME "homeplate2"
#elif defined(ARDUINO_INKPLATECOLOR)
#define HOSTNAME "homeplate6"
#elif defined(ARDUINO_INKPLATE10) || defined(ARDUINO_INKPLATE10V2)
#define HOSTNAME "homeplate10"
#endif
#endif

// How long to sleep between image refreshes
#define TIME_TO_SLEEP_MIN 20

// Static IP information
// If unset uses DHCP, but updates may be slower, set to use a Static IP
// #define STATIC_IP "192.168.1.10"
// #define STATIC_SUBNET "255.255.255.0"
// #define STATIC_GATEWAY "192.168.1.1"
// #define STATIC_DNS "192.168.1.1"

// NTP Time server to set RTC
#define NTP_SERVER "NTP Server IP"

// How often to re-sync the clock to NTP
#define NTP_SYNC_INTERVAL (24*60)/TIME_TO_SLEEP_MIN // ~ once a day when updating every TIME_TO_SLEEP_MIN minutes

// URL of PNG image to display (must have the extension in it)
#define IMAGE_URL "HTTP URL of dashboard screenshot to display"


// MQTT Broker
#define MQTT_HOST "MQTT Broker IP"
#define MQTT_PORT 1883
// Set MQTT_USER & MQTT_PASSWORD if needed
//#define MQTT_USER "mqtt username"
//#define MQTT_PASSWORD "mqtt password"
// Customize node id and device name if needed
//#define MQTT_NODE_ID "homeplate6"	// defaults to HOSTNAME
//#define MQTT_DEVICE_NAME "HomePlate6"	// defaults to HOSTNAME


// Timezone
// see timezone_config.h for options
#define TIMEZONE_UTC


// keep this to signal the program has a valid config file
#define CONFIG_H