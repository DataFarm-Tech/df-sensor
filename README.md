# ESP32 Bridge Node DataFarm (2.4Ghz Wifi)

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

