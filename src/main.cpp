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
    pthread_attr_t attr_capture, attr_send;

    pthread_attr_init(&attr_capture);
    pthread_attr_setstacksize(&attr_capture, THREAD_STACK_SIZE);
    pthread_attr_init(&attr_send);
    pthread_attr_setstacksize(&attr_send, THREAD_STACK_SIZE);

    delay(2000);

    pthread_create(&th_capture, &attr_capture, capture, NULL);
    pthread_create(&th_send, &attr_send, send, NULL);

    // Destroy the thread attributes when no longer needed
    pthread_attr_destroy(&attr_capture);
    pthread_attr_destroy(&attr_send);

    pthread_join(th_capture, NULL);
    pthread_join(th_send, NULL);
}

void loop() {}

void* capture(void*)
{
    data sensorData;
    sensorData.nodeId = NODE_ID; // Convert NODE_ID to std::string

    while (1) 
    {
        sensorData.moisture = getMoisture();
        sensorData.ph = getPh();

        pthread_mutex_lock(&queueMutex);
        globalQueue.push(sensorData);
        size_t queueSize = globalQueue.size(); // Get the number of elements in the queue
        pthread_mutex_unlock(&queueMutex);
        printf("Capturing Data: %.2f, pH: %.2f, nodeId: %s, queueSize: %zu\n", sensorData.moisture, sensorData.ph, sensorData.nodeId.c_str(), queueSize);
        delay(4000);
    }
}

void* send(void*)
{
    LoRa.setPins(ss, rst, dio0);
    
    while (!LoRa.begin(915E6)) 
    {
        Serial.println("LoRa module is not connected or wired incorrectly!");
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
            printf("Sending Data: %.2f, pH: %.2f, nodeId: %s\n", recData.moisture, recData.ph, recData.nodeId.c_str());
            globalQueue.pop();
        }
        pthread_mutex_unlock(&queueMutex);
        delay(4000); // Simulate some work
    }
}


#include <cstdlib> // Include the necessary header for generating random numbers

float getPh()
{
    // Generate a random number between 0 and 128
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 128.0f;
}

float getMoisture()
{
    // Generate a random number between 0 and 128
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 128.0f;
}
