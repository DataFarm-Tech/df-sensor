#include <cstdlib> // Include the necessary header for generating random numbers
#include <pthread.h>
#include <queue>
#include <SPI.h>
#include <iostream>
#include <Arduino.h>
#include <LoRa.h>

#define THREAD_STACK_SIZE 8192 // Adjust the stack size as needed

#define ss 5
#define rst 25
#define dio0 2

#ifndef NODE_ID
#define NODE_ID "test"  // Default nodeId if not defined
#endif

typedef struct {
    float moistureLevel1;
    float moistureLevel2;
    float moistureLevel3;
    float phLevel1;
    float phLevel2;
    float phLevel3;
    std::string nodeId;
} data;

std::queue<data> globalQueue;
pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;

float getMoisture(void);
float getPh(void);

void* send(void*);
void* capture(void*);

void setup(void)
{
    LoRa.setPins(ss, rst, dio0); //initalises lora pins define at top of file

    while (!LoRa.begin(915E6)) //Pause until lora connection is correct
    {
        printf("LoRa module is not connected or wired incorrectly!\n");
        delay(500);
    }

    LoRa.setSyncWord(0xF3); //set channel
    Serial.begin(115200);

    pthread_t th_capture;
    pthread_t th_send;
    pthread_attr_t attr_capture, attr_send;

    pthread_attr_init(&attr_capture);
    pthread_attr_setstacksize(&attr_capture, THREAD_STACK_SIZE);
    pthread_attr_init(&attr_send);
    pthread_attr_setstacksize(&attr_send, THREAD_STACK_SIZE);

    delay(2000);

    pthread_create(&th_capture, &attr_capture, capture, NULL);
    pthread_create(&th_send, &attr_send, send, NULL);

    pthread_attr_destroy(&attr_capture);
    pthread_attr_destroy(&attr_send);

    pthread_join(th_capture, NULL);
    pthread_join(th_send, NULL);
}

void loop(void) {}

void* capture(void*)
{
    data sensorData; //create sensorData struct 
    sensorData.nodeId = NODE_ID; //set structs nodeId attr to the nodeId given at compilation time

    while (1) //infinite loop
    {
        sensorData.moistureLevel1 = getMoisture(); //Getting moisture from GPIO pins (getMoisture)
        sensorData.moistureLevel2 = getMoisture(); //Getting moisture from GPIO pins (getMoisture)
        sensorData.moistureLevel3 = getMoisture(); //Getting moisture from GPIO pins (getMoisture)

        sensorData.phLevel1 = getPh(); //Getting ph from GPIO pins (getPh)
        sensorData.phLevel2 = getPh(); //Getting ph from GPIO pins (getPh)
        sensorData.phLevel3 = getPh(); //Getting ph from GPIO pins (getPh)

        pthread_mutex_lock(&queueMutex); //Lock Mutex
        size_t queueSize = globalQueue.size(); // Get the number of elements in the queue
        globalQueue.push(sensorData); //add struct to queue
        printf("Sending Data to Queue: moistureLevel1: %.2f, moistureLevel2: %.2f, moistureLevel3: %.2f, pHLevel1: %.2f, pHLevel2: %.2f, pHLevel3: %.2f, nodeId: %s\n", sensorData.moistureLevel1, sensorData.moistureLevel2, sensorData.moistureLevel3, sensorData.phLevel1, sensorData.phLevel2, sensorData.phLevel3, sensorData.nodeId.c_str());
        pthread_mutex_unlock(&queueMutex); //Unlock Mutex
        
        delay(60000); //measure every 5 seconds
    }
}

void* send(void*)
{
    data recData;
    while (1) 
    {
        pthread_mutex_lock(&queueMutex);
        if (!globalQueue.empty()) 
        {
            recData = globalQueue.front();
            std::string message = std::to_string(recData.moistureLevel1) + "," + std::to_string(recData.moistureLevel2) + "," + std::to_string(recData.moistureLevel3) + ',' + std::to_string(recData.phLevel1) + ',' + std::to_string(recData.phLevel2) + ',' + std::to_string(recData.phLevel3) + ',' + recData.nodeId;
            LoRa.beginPacket();
            LoRa.print(message.c_str());
            LoRa.endPacket();
            printf("Sending Data via LoRa: moistureLevel1: %.2f, moistureLevel2: %.2f, moistureLevel3: %.2f, pHLevel1: %.2f, pHLevel2: %.2f, pHLevel3: %.2f, nodeId: %s\n", recData.moistureLevel1, recData.moistureLevel2, recData.moistureLevel3, recData.phLevel1, recData.phLevel2, recData.phLevel3, recData.nodeId.c_str());
            globalQueue.pop();
        }
        else
        {
            printf("Queue is Empty: Nothing to Send\n");
            delay(200);
        }
        pthread_mutex_unlock(&queueMutex);
        delay(20);
    }
}



float getPh(void)
{
    // Generate a random number between 1 and 14
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 14.1f;
}

float getMoisture(void)
{
    // Generate a random number between 0 and 100
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100.0f;
}
