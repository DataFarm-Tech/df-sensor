# ESP32 Bridge Node DataFarm (2.4Ghz Wifi)


## The following below is the pinout, DO NOT CHANGE EVER.

| From Device | From Pin          | To Device   | To Pin         |
| ----------- | ----------------- | ----------- | -------------- |
| ESP32       | GPIO 5            | RFM95W      | NSS/CS         |
| ESP32       | GPIO 23           | RFM95W      | MOSI           |
| ESP32       | GPIO 19           | RFM95W      | MISO           |
| ESP32       | GPIO 18           | RFM95W      | SCK            |
| ESP32       | GPIO 14           | RFM95W      | RST            |
| ESP32       | GPIO 27           | RFM95W      | DIO0/IRQ       |
| ESP32       | 3.3V              | RFM95W      | VCC            |
| ESP32       | GND               | RFM95W      | GND            |
| ESP32       | GPIO 16           | SP3485      | TX-0           |
| ESP32       | GPIO 17           | SP3485      | RX-1           |
| ESP32       | GPIO 4            | SP3485      | RTS            |
| ESP32       | 3.3V              | SP3485      | 3-5V           |
| ESP32       | GND               | SP3485      | GND            |
| ESP32       | USB 5V            | Soil Sensor | Brown (Power+) |
| ESP32       | GND               | Soil Sensor | Black (Power-) |
| Soil Sensor | Yellow/Green (A+) | SP3485      | A              |
| Soil Sensor | Blue (B-)         | SP3485      | B              |

# NOTE: PLEASE ADD A 120 OHM RESISTOR PARALLEL TO THE A+ AND B- PIN OF THE SOIL SENSOR (DECOUPLING RESISTOR)

## PINOUT diagram for SENSOR V1 PROTOTYPE : ESP32-WROOM-E/ESP32-WROOM-DEVKIT/ESP32S-WROOM
(will add soon)
## PINOUT diagram for PCB - ESP32-S3 
(will add soon)
#

## Reference to RF95 lib
https://www.airspayce.com/mikem/arduino/RadioHead/classRH__RF95.html


Sub routine (init):
- Set BRIDGE_FIRM_VER
- Set BRIDGE_HW_VER
- Set BRIDGE_ADDRESS
- ssid of network + password
- Set loraWAN module freq to 915mhz
- set power of lora module (db) (transmitter power set to 5dbm)- radiohead (RHMesh.h)
    - node address (16bit unsigned int)
    - create a constant max message length (char/int) 50 


if init success (bool):
    print to console success
else
    print to console fail

Send HTTP API request bridge node is connected

Start LoraWAN scanning (void loop check for incoming packets)

When packet is found then it is Enqeued into queue

Delimit string by a comma, sub-string array

Convert into JSON object

Send to API via http requests

Dequeue

Repeat (if no packets are found then sleep)



Sending/Scanning Loops 


Request
- String (first second third)
first: {1,2,3} 
1) Testing (STATUS, BATTERY_LEVEL, SOLAR_PANEL_STATUS, SOIL DATA, NODE_ADDRESS, SEN_FIRM_VER, SERIAL_NUM) //tools does not connect to backend


2) Data (NODEID, USERID, PH, MOISTURE)
second: 



Note: Data cleaning: remove soil temperature readings if below x (x is min temp)

