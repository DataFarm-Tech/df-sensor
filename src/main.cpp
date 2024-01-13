#include <pthread.h>
#include <queue>
#include <SPI.h>
#include <iostream>
#include <Arduino.h>
#include "eeprom.h"
#include <LoRa.h>

#define ss 5
#define rst 14
#define dio0 2

typedef struct {
    float moisture;
    float ph;
    std::string nodeId;
} data;

std::queue<data> globalQueue;
pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;

float getMoisture();
float getPh();


void* send(void*);
void* capture(void*);

void setup()
{
    Serial.begin(115200);

    pthread_t th_capture;
    pthread_t th_send;

    delay(2000);

    pthread_create(&th_capture, NULL, capture, NULL);
    pthread_create(&th_send, NULL, send, NULL);

    pthread_join(th_capture, NULL);
    pthread_join(th_send, NULL);
}

void loop() {}

void* capture(void*)
{
    data sensorData;
    eeprom();
    eeprom::restoreEEPROM();
    sensorData.nodeId = eeprom::getNodeId();

    while (1) 
    {
        sensorData.moisture = getMoisture();
        sensorData.ph = getPh();

        printf("Sending Moisture: %.2f, pH: %.2f, nodeId: %s\n", sensorData.moisture, sensorData.ph, sensorData.nodeId.c_str());

        pthread_mutex_lock(&queueMutex);
        globalQueue.push(sensorData);
        pthread_mutex_unlock(&queueMutex);
        delay(60000);
    }
}

void* send(void*)
{
    LoRa.setPins(ss, rst, dio0);
    
    while (!LoRa.begin(915E6)) 
    {
        Serial.println(".");
        delay(500);
    }

    LoRa.setSyncWord(0xF3);
    data recData;
    while (1) 
    {
        pthread_mutex_lock(&queueMutex);
        if (!globalQueue.empty()) 
        {
            recData = globalQueue.front();
            std::string message = std::to_string(recData.moisture) + "," + std::to_string(recData.ph) + "," + recData.nodeId;
            LoRa.beginPacket();
            LoRa.print(message.c_str());
            LoRa.endPacket();
            Serial.print("Sending LoRa packet: ");
            Serial.println(message.c_str());
        
            globalQueue.pop();
        }
        pthread_mutex_unlock(&queueMutex);
        delay(30000); // Simulate some work
    }
}


float getPh()
{
    // Read data from GPIO pins
    return 3.6;
}

float getMoisture()
{
    // Read data from GPIO pins
    return 3.9;
}


