# ntp-clock [![Build Status](https://travis-ci.org/boneskull/ntp-clock.svg)](https://travis-ci.org/boneskull/ntp-clock)

> 24-hour NTP clock for ESP8266

## Screenshots

None yet; I need to build an enclosure and make it presentable!

## Prerequisites

- Python 2.7 w/ `pip`
- [PlatformIO](http://platformio.org) ([install](http://platformio.org/#!/get-started))

If using a [WeMos D1 Mini](http://www.wemos.cc/wiki/doku.php?id=en:d1_mini), don't forget to install any necessary drivers.

## BOM

- One (1) ESP8266-based board.  This was built with a WeMos D1 Mini ($4), but theoretically any board will work.
- One (1) 4-digit 7-segment display module w/I2C akin to [this one](https://www.adafruit.com/products/878), but any similar thing on AliExpress is likely adequate
- One (1) micro USB cable to power the D1 Mini
- Four (4) M/F jumpers, DuPont cables, etc.
- Whatever headers you might need for the board; in my case I soldered two (2) 8-pin female headers to the D1 Mini
- Enclosure of your preference

## Wiring

- ESP8266 GND -> Display module GND
- ESP8266 3V -> Display module VCC
- ESP8266 pin 4 -> Display module SDA
- ESP8266 pin 5 -> Display module SCL

Note that the ESP8266 pin number *may not necessarily correspond* to the number printed on your board.  On the D1 Mini, for example, pin 4 is "D2" and pin 5 is "D1".  RTFM.

## Configuration

1.  Clone this repo, then modify `platform.ini` to your liking ([docs](http://docs.platformio.org/en/latest/projectconf.html)).  If you are using a different board, please add an appropriate environment entry (and send a PR!).  You'll want to copy the value(s) for `lib_install`.
2.  Modify `src/ntp-clock.cpp` and adjust the definitions as necessary:

  - `SSID`: Whatever network you want to connect to
  - `PASS`: Password of the network
  - `TZ`: Your time zone offset with respect to UTC
  - `HOST`: An NTP server
  
3.   Please do not commit your credentials and upload them to GitHub.

## Flashing

1. Clone this repo
2. Enter your working copy directory
3. To build with the default environment (with a D1 Mini), execute:

  ```shell
  $ platformio run
  ```
  
  If this was successful, then we can upload:
  
  ```shell
  $ platformio run --target upload
  ```
  
  You can specify a custom environment name with the `--environment=<name>` argument.
4. If everything worked, the time separator on the display should blink a few times (this signifies it is negotiating with your AP), then you should see a time.  The time separator will continue to blink.  Ooh!

> PRO TIP: If you're having `esptool` flake, try [`esptool.py`](https://github.com/themadinventor/esptool) instead.

### Alternatives

You can also *theoretically* load `ntp-clock.cpp` in the Arduino IDE (it will rename the file and put it in a new directory) and attempt compilation/flashing there.  You'll need to add/configure your board manually, and ensure the following libs are installed:

  - [NTPClient](https://github.com/FWeinb/NTPClient)
  - [Adafruit_GFX](https://github.com/adafruit/Adafruit_GFX)
  - [Adafruit_LED_Backpack](https://github.com/adafruit/Adafruit_LED_Backpack)
  
## License

Copyright 2016 [Christopher Hiller](https://boneskull.com).  Licensed MIT.
