/**
 * 24-hour NTP clock
 * For use with a 4-digit 7-segment display and an ESP8266
 * @copyright 2016 Christopher Hiller <boneskull@boneskull.com>
 * @license MIT
 */

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <Adafruit_LEDBackpack.h>
#include <Task.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#define SSID "YOUR WIFI NETWORK"
#define PASS "AND ITS PASSWORD"
#define TZ -8 // your time zone offset
#define HOST "time.apple.com" // your favorite NTP server

TaskManager taskManager;
NTPClient timeClient(HOST, TZ * 3600);
Adafruit_7segment matrix = Adafruit_7segment();

/**
 * Is the colon supposed to be on or off?
 */
bool colon = true;

/**
 * Check this in the loop; if it differs from what NTPClient tells us
 * then it's time to update the time.
 */
unsigned long prevMin = 0;

void toggleColon(uint32_t delta);
void updateTime(uint32_t delta);
void waitForConnection(uint32_t delta);
void keepConnected(uint32_t delta);

FunctionTask taskToggleColon(toggleColon, MsToTaskTime(500));
FunctionTask taskUpdateTime(updateTime, MsToTaskTime(1000));
FunctionTask taskWaitForConnection(waitForConnection, MsToTaskTime(250));
FunctionTask taskKeepConnected(keepConnected, MsToTaskTime(250));

/**
 * Toggles the colon state and writes the display.
 */
void toggleColon(uint32_t delta) {
  colon = !colon;
  matrix.drawColon((boolean) colon);
  matrix.writeDisplay();
}

/**
 * Resets 7-segment display.
 */
void reset() {
  matrix.clear();
  colon = false;
  matrix.writeDisplay();
}

/**
 * Returns `true` if WiFi is offline.
 */
bool offline() {
  return WiFi.status() != WL_CONNECTED;
}

/**
 * Pauses normal behavior and waits until we have a WiFi connection again,
 * then resumes.
 */
void waitForConnection(uint32_t delta) {
  toggleColon(delta);
  if (!offline()) {
    Serial.println("Connected to " + String(SSID));
    taskManager.StopTask(&taskWaitForConnection);
    taskManager.StartTask(&taskToggleColon);
    taskManager.StartTask(&taskUpdateTime);
    taskManager.StartTask(&taskKeepConnected);
  }
}

/**
 * Connects to WiFi network.  Flashes colon quickly during this phase.
 */
void keepConnected(uint32_t delta) {
  if (offline()) {

    taskManager.StopTask(&taskKeepConnected);
    taskManager.StopTask(&taskToggleColon);
    taskManager.StopTask(&taskUpdateTime);

    Serial.println("Connecting to " + String(SSID));
    WiFi.begin(SSID, PASS);

    taskManager.StartTask(&taskWaitForConnection);
  }
}

/**
 * Shortcut to turn a char into an int.
 */
uint8_t charToInt(char c) {
  return (uint8_t) c - '0';
}

/**
 * Draws the time.  Make adjustments for single-digit values.
 */
void drawTime(String hours, String minutes) {
  if (hours.length() == 1) {
    matrix.writeDigitRaw(0, 0);
    matrix.writeDigitNum(1, charToInt(hours.charAt(0)));
  } else {
    matrix.writeDigitNum(0, charToInt(hours.charAt(0)));
    matrix.writeDigitNum(1, charToInt(hours.charAt(1)));
  }
  if (minutes.length() == 1) {
    matrix.writeDigitNum(3, 0);
    matrix.writeDigitNum(4, charToInt(minutes.charAt(0)));
  } else {
    matrix.writeDigitNum(3, charToInt(minutes.charAt(0)));
    matrix.writeDigitNum(4, charToInt(minutes.charAt(1)));
  }
}

/**
 * If a second or more has passed, ensure WiFi connectivity, then check the NTP
 * client for the time.  Note the client will not actually ping the host more
 * than every 60s as configured.
 *
 * If the time's "minute" value is not equal to the current value we're
 * displaying, then draw the new time and save its value.
 */
void updateTime(uint32_t delta) {
  timeClient.update();
  unsigned long min = (timeClient.getRawTime() % 3600) / 60;
  if (min != prevMin) {
    drawTime(timeClient.getHours(), timeClient.getMinutes());
    Serial.println("Updating time to " + timeClient.getFormattedTime());
    prevMin = min;
  }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

/**
 * Initializes matrix and connects to WiFi.
 */
void setup() {
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
  matrix.begin(0x70);
  reset();
  taskManager.StartTask(&taskToggleColon);
  taskManager.StartTask(&taskUpdateTime);
  taskManager.StartTask(&taskKeepConnected);
}

/**
 * Flashes colon on/off every second.
 * Asks the NTP client for an update every second; if the minute has changed,
 * then update the display.
 * Keep WiFi connection alive.
 */
void loop() {
  taskManager.Loop();
}

#pragma clang diagnostic pop
