#include "lora.h"
#include "capture.h"
extern QueueHandle_t xQueue;


void loraSend(void *pvParameters) 
{
    //NOT WORKING ATM, SETUP FOR LORA COMMS FAILING NOT RECOGNISING LORA OBEJCT
    //ERROR:  error: 'LoRa' was not declared in this scope

    // Serial.println("LoRa Receiver");
    // while (!LoRa.begin(915E6)) {
    //     Serial.println(".");
    //     delay(500);
    // }
    // LoRa.setPins(ss, rst, dio0);


    // LoRa.setSyncWord(0xF3);
    // Serial.println("LoRa Initializing OK!");
    data receivedData;


    while (1) 
    {
        if (xQueueReceive(xQueue, &receivedData, portMAX_DELAY)) 
        {
            // Concatenate the data into a string
            std::string strPacket = std::to_string(receivedData.moisture) + "," + std::to_string(receivedData.ph) + "," + receivedData.nodeId;

            //THIS CODE SHOULD WORK
            printf("Received: %s\n", strPacket.c_str());
            // LoRa.beginPacket();
            // LoRa.print(strPacket.c_str());
            // LoRa.endPacket();
            Serial.println(strPacket.c_str());
            delay(10000);
        }
    }
}