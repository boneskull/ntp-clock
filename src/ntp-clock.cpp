/**
 * 24-hour NTP clock
 * For use with a 4-digit 7-segment display and an ESP8266
 * @copyright 2016 Christopher Hiller <boneskull@boneskull.com>
 * @license MIT
 */

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <Adafruit_LEDBackpack.h>

#define SSID "YOUR WIFI NETWORK"
#define PASS "AND ITS PASSWORD"
#define TZ -8 // your time zone offset
#define HOST "time.apple.com" // your favorite NTP server

NTPClient timeClient(HOST, TZ * 3600);
Adafruit_7segment matrix = Adafruit_7segment();

/**
 * Is the colon supposed to be on or off?
 */
bool colon = true;

/**
 * Use these to avoid calls to delay() during the loop
 */
unsigned long prevColonMillis = 0;
unsigned long prevUpdateMillis = 0;

/**
 * Check this in the loop; if it differs from what NTPClient tells us
 * then it's time to update the time.
 */
unsigned long prevMin = 0;

/**
 * Toggles the colon state and writes the display.
 */
void toggleColon() {
  colon = !colon;
  matrix.drawColon((boolean)colon);
  matrix.writeDisplay();
}

/**
 * Resets 7-segment display.
 */
void reset() {
  matrix.clear();
  colon = (boolean) false;
  matrix.writeDisplay();
}

/**
 * Returns `true` if WiFi is offline.
 */
bool offline() {
  return WiFi.status() != WL_CONNECTED;
}

/**
 * Connects to WiFi network.  Flashes colon during this phase.
 */
void connectWiFi() {
  if (offline()) {
    WiFi.begin(SSID, PASS);

    Serial.println("Connecting to " + String(SSID));
    while (offline()) {
      Serial.print('.');
      toggleColon();
      delay(250);
    }

    Serial.println("Connected to " + String(SSID));
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
 * If a half-second or more has passed, toggle the colon display.
 * This should be run *after* the time has updated, because toggleColon()
 * is responsible for writing the display.
 */
void maybeToggleColon(unsigned long millis) {
  unsigned long colonDelta = millis - prevColonMillis;

  if (colonDelta >= 500) {
    toggleColon();
    prevColonMillis = millis;
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
void maybeUpdateTime(unsigned long millis) {
  unsigned long updateDelta = millis - prevUpdateMillis;
  if (updateDelta >= 1000) {
    connectWiFi();
    timeClient.update();
    unsigned long min = (timeClient.getRawTime() % 3600) / 60;
    if (min != prevMin) {
      drawTime(timeClient.getHours(), timeClient.getMinutes());
      Serial.println("Updating time to " + timeClient.getFormattedTime());
      prevMin = min;
    }
    prevUpdateMillis = millis;
  }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

/**
 * Initializes matrix and connects to WiFi.
 */
void setup() {
  Serial.begin(115200);
  matrix.begin(0x70);
  reset();
  connectWiFi();
  reset();
}

/**
 * Flashes colon on/off every second.
 * Asks the NTP client for an update every second; if the minute has changed,
 * then update the display.
 * Keep WiFi connection alive.
 */
void loop() {
  maybeToggleColon(millis());
  maybeUpdateTime(millis());
  // call it a second time since the last operation may have taken >= 500ms
  maybeToggleColon(millis());
}

#pragma clang diagnostic pop
