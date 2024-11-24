#ifndef CONFIG_H
#define CONFIG_H

#ifndef ID
#define ID "1G36S8" // Test value
#endif

// Pin definition for the SP3485:
#define RS485_RX 16
#define RS485_TX 17
#define RS485_RTS 4
#define RS485_BAUD 4800 

// LORA CONFIG
#define RFM95_SS 5 // CS
#define RFM95_RST 14 // RST
#define RFM95_DIO0 27 // DIO0
#define RFM95_MOSI 23
#define RFM95_MISO 19
#define RFM95_SCK 18
#define RF95_FREQ 915.0 // DO NOT CHANGE FROM 915MHZ

#define MESH_ADDRESS 0x00000002  // 32-bit address, unique for each node
#define MAX_NODES 0xFFFFFFFF     // Maximum number of nodes with 32-bit addressing
#define MAX_TTL 10               // Maximum number of hops a message can take

#endif //CONFIG_H