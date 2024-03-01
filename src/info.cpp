#include "homeplate.h"

#define COL1_NAME_X 1 * (E_INK_WIDTH / 8)
#define COL1_DATA_X 2 * (E_INK_WIDTH / 8)
#define COL2_NAME_X 5 * (E_INK_WIDTH / 8)
#define COL2_DATA_X 6 * (E_INK_WIDTH / 8)

const static int lineHeight = 20;

const char *wl_status_to_string(wl_status_t status)
{
  switch (status)
  {
  case WL_NO_SHIELD:
    return "NO_SHIELD";
  case WL_IDLE_STATUS:
    return "IDLE_STATUS";
  case WL_NO_SSID_AVAIL:
    return "NO_SSID_AVAIL";
  case WL_SCAN_COMPLETED:
    return "SCAN_COMPLETED";
  case WL_CONNECTED:
    return "CONNECTED";
  case WL_CONNECT_FAILED:
    return "CONNECT_FAILED";
  case WL_CONNECTION_LOST:
    return "CONNECTION_LOST";
  case WL_DISCONNECTED:
    return "DISCONNECTED";
  }
  return "UNKNOWN";
}

void displayBoundaryBox()
{
  display.fillRect(0, 0, 10, E_INK_HEIGHT, BLACK);                // left
  display.fillRect(E_INK_WIDTH - 10, 0, 10, E_INK_HEIGHT, BLACK); // right
  display.fillRect(0, 0, E_INK_WIDTH, 10, BLACK);                 // top
  display.fillRect(0, E_INK_HEIGHT - 10, E_INK_WIDTH, 10, BLACK); // bottom
}

