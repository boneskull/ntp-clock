cmake_minimum_required(VERSION 3.2)
project(ntp-clock)

set(PLATFORMIO_CMD "/usr/local/bin/platformio")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/variants/d1_mini")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/cores/esp8266")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/cores/esp8266/spiffs")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266WiFi/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266WiFi/src/arch")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266WiFi/src/include")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266WiFi/src/lwip")
include_directories("$ENV{HOME}/.platformio/lib/NTPClient_ID551")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/Wire")
include_directories("$ENV{HOME}/.platformio/lib/Adafruit_GFX_ID13")
include_directories("$ENV{HOME}/.platformio/lib/Adafruit_GFX_ID13/Fonts")
include_directories("$ENV{HOME}/.platformio/lib/Adafruit_LED_Backpack_ID25")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/tools/sdk/include")
include_directories("./lib/Task")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ArduinoOTA")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/DNSServer/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/Dummy/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/EEPROM")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266AVRISP/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266HTTPClient/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266HTTPUpdateServer/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266SSDP")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266WebServer/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266WiFiMesh/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266httpUpdate/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/ESP8266mDNS")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/Ethernet/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/GDBStub/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/Hash/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/OneWire")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/SD/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/SPI")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/Servo/src")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/SoftwareSerial")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/TFT_Touch_Shield_V2")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/Ticker")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoespressif/libraries/esp8266/src")
include_directories("$ENV{HOME}/.platformio/packages/toolchain-xtensa/xtensa-lx106-elf/include")
include_directories("$ENV{HOME}/.platformio/packages/toolchain-xtensa/lib/gcc/xtensa-lx106-elf/4.8.2/include")
include_directories("$ENV{HOME}/.platformio/packages/toolchain-xtensa/lib/gcc/xtensa-lx106-elf/4.8.2/include-fixed")

add_definitions(-DF_CPU=80000000L)
add_definitions(-D__ets__)
add_definitions(-DICACHE_FLASH)
add_definitions(-DESP8266)
add_definitions(-DARDUINO_ARCH_ESP8266)
add_definitions(-DARDUINO_ESP8266_ESP12)
add_definitions(-DARDUINO=200)
add_definitions(-DPLATFORMIO=020804)

add_custom_target(
    PLATFORMIO_BUILD ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_UPLOAD ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target upload
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_CLEAN ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target clean
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_PROGRAM ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target program
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_UPLOADFS ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target uploadfs
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_UPDATE_ALL ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion update
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(ntp-clock
    src/ntp-clock.cpp
)
