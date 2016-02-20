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
boolean colon = (boolean) true;

/**
 * Use this to avoid calls to delay() during the loop
 */
unsigned long prevMillis = 0;

/**
 * Check this in the loop; if it differs from what NTPClient tells us
 * then it's time to update the time.
 */
unsigned long prevMin = 0;

/**
 * Toggles the colon state and writes the display.
 */
void toggleColon() {
  colon = (boolean) !colon;
  matrix.drawColon(colon);
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
 * Connects to WiFi network.  Flashes colon during this phase.
 */
void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  reset();

  WiFi.begin(SSID, PASS);

  Serial.println("Connecting to " + String(SSID));
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    toggleColon();
    delay(250);
  }

  Serial.println("Connected to " + String(SSID));

  reset();
}

/**
 * Initializes matrix and connects to WiFi.
 */
void setup() {
  Serial.begin(115200);
  matrix.begin(0x70);
  connectWiFi();
}

/**
 * Shortcut to turn a char into an int.
 */
uint8_t charToInt(char c) {
  return (uint8_t) c - '0';
}

/**
 * Draws the time.  This is 24-hour time, but the hour may be one or two
 * digits; leave the first blank if there's only one digit.
 */
void drawTime(String hours, String minutes) {
  if (hours.length() == 1) {
    matrix.writeDigitRaw(0, 0);
    matrix.writeDigitNum(1, charToInt(hours.charAt(0)));
  } else {
    matrix.writeDigitNum(0, charToInt(hours.charAt(0)));
    matrix.writeDigitNum(1, charToInt(hours.charAt(1)));
  }
  matrix.writeDigitNum(3, charToInt(minutes.charAt(0)));
  matrix.writeDigitNum(4, charToInt(minutes.charAt(1)));
}

/**
 * Flashes colon on/off every second.
 * Asks the NTP client for an update every second; if the minute has changed,
 * then update the display.
 * Keep WiFi connection alive.
 */
void loop() {
  unsigned long currentMillis = millis();
  unsigned long delta = currentMillis - prevMillis;

  if (delta >= 500) {
    if (delta >= 1000) {
      connectWiFi();

      timeClient.update();

      unsigned long min = (timeClient.getRawTime() % 3600) / 60;
      if (min != prevMin) {
        drawTime(timeClient.getHours(), timeClient.getMinutes());
        Serial.println("Updating time to " + timeClient.getFormattedTime());
      }
      prevMin = min;
    }

    prevMillis = currentMillis;

    toggleColon();
  }
}