# DripMatic - Automated Irrigation System

## Goal of the project
DripMatic is a solar powered mikrocontroller-based automated irrigation system for the small and middle sized outdoor vegetables production. Its WSAN (wireless sensor-actor-network) collects through smart-sensors crop specific data i.e.: soil-moisture, humidity, temperature, rainfall, wind direction and wind speed etc. The collected data will be compared with pre-defined parameter in a database. After calculating the amount of water which needs to be applied, the system analysis the weather data and decide the irrigation time and duration. Crop specific data will be stored on an SD-Card and afterwards transmitted to a server if available.

## hardware requirements

- Arduino UNO Rev.3
- DS1302 RTC-Module
- SPI MicroSD-Card Reader
- LCD-Module 4x20 with ST7066U-0A Controller
- I2C PCF8574 module
- MPX 5100DP Pressure Transducer
- 3 x LEDs (red, yellow, green)
- ESP8266 Wifi Development Board


## docs
this folder will contain all developed and designed hardware components like:
- stl-files for printing parts (i.e. mounting for pressure transducer, moulds for seal rings etc.)
- PCBs for sensor- and actor nodes
- knowledge base (i.e. cultivating techniques, test data record etc.)


