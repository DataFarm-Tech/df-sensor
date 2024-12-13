#ifndef CONFIG_H
#define CONFIG_H

//The following header file, holds all the compile time variables from the compile process in the python scripts. See python scripts uinder build/ for my information.
//Variables such as:
//  - Network Config
//  - HTTP API Config
//  - LoRa Config
//All are kept here, for other source files to use these variables, it must be included in the source file.
//If compile time variable is not passed in then the variable will use it's default value.

#include "types.h"

//The following variable declares the UID of the bridge.
#ifndef ID
#define ID "TEST"
#endif

#ifndef SSID_CRED
#define SSID_CRED "MyWifi"
#endif

#ifndef PASS_CRED
#define PASS_CRED "ExpZZLN9U8V2"
#endif

//This variable indicates if the OTA feature is permitted to operate. (For development purposes this functtion can be disabled)
#ifndef OTA_ENABLED
#define OTA_ENABLED 1
#endif

//HTTP CONFIG
#ifndef API_KEY
#define API_KEY "8151D74D23320A035F6DC0717F54B978BE3A29100AE52E716170D8A84B939BAC5C14EE537D1174919F67BA916E29D4DB696E73B4296E5F6FD2BE16984B1D31F1" //DEFAULT KEY
#endif

#define PORT 8001
#define PORT_STR "80"  // Add this line for string concatenation
#define HOSTNAME "https://194.195.255.90:443"

// LORA CONFIG
#define RFM95_CS 5 // CS
#define RFM95_RST 14 // RST
#define RFM95_INT 27 // DIO0
#define RFM95_MOSI 23
#define RFM95_MISO 19
#define RF95_FREQ 915.0 // DO NOT CHANGE FROM 915MHZ

#define MESH_ADDRESS 0x00000001  // 32-bit address, unique for each node
#define MAX_NODES 0xFFFFFFFF     // Maximum number of nodes with 32-bit addressing
#define MAX_TTL 10               // Maximum number of hops a message can take

//Globals variable declaration
extern controller_state currentState;

#define POST_MEAS_DATA HOSTNAME "/controller/add/data"
#endif  // CONFIG_H