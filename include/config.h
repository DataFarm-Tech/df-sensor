#ifndef CONFIG_H
#define CONFIG_H


#ifndef ID
#define ID "TEST"
#endif

// LORA CONFIG
#define RFM95_CS 5 // CS
#define RFM95_RST 25 // RST
#define RFM95_INT 2 // DIO0
#define RF95_FREQ 915.0 // DO NOT CHANGE FROM 915MHZ

#define MESH_ADDRESS 0x00000001  // 32-bit address, unique for each node
#define MAX_NODES 0xFFFFFFFF     // Maximum number of nodes with 32-bit addressing
#define MAX_TTL 10               // Maximum number of hops a message can take

#endif //CONFIG_H