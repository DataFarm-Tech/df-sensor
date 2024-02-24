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

    // Destroy the thread attributes when no longer needed
    pthread_attr_destroy(&attr_capture);
    pthread_attr_destroy(&attr_send);

    pthread_join(th_capture, NULL);
    pthread_join(th_send, NULL);
}

void loop() {}

void* capture(void*)
{
    data sensorData; //create sensorData struct 
    sensorData.nodeId = NODE_ID; //set structs nodeId attr to the nodeId given at compilation time

    while (1) //infinite loop
    {
        sensorData.moisture = getMoisture(); //Getting moisture from GPIO pins (getMoisture)
        sensorData.ph = getPh(); //Getting ph from GPIO pins (getPh)

        pthread_mutex_lock(&queueMutex); //Lock Mutex
        size_t queueSize = globalQueue.size(); // Get the number of elements in the queue
        
        // if (queueSize < 10) //If the queue size is greater than 10 then halt addition to queue
        // {
            globalQueue.push(sensorData); //add struct to queue
            printf("Capturing Data: moisture: %.2f, pH: %.2f, nodeId: %s, queueSize: %zu\n", sensorData.moisture, sensorData.ph, sensorData.nodeId.c_str(), queueSize);
        // }
        // else
        // {
        //     printf("An error has occured, to many items in queue. Halting addition to queue!!\n");
        // }

        pthread_mutex_unlock(&queueMutex); //Unlock Mutex
        
        delay(5000); //measure every 5 seconds
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
            std::string message = std::to_string(recData.moisture) + "," + std::to_string(recData.ph) + "," + recData.nodeId;
            LoRa.beginPacket();
            LoRa.print(message.c_str());
            LoRa.endPacket();
            printf("Sending Data: moisture: %.2f, pH: %.2f, nodeId: %s\n", recData.moisture, recData.ph, recData.nodeId.c_str());
            globalQueue.pop();
        }
        pthread_mutex_unlock(&queueMutex);
        delay(4000); // Simulate some work
    }
}



float getPh()
{
    // Generate a random number between 1 and 14
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 14.1f;
}

float getMoisture()
{
    // Generate a random number between 0 and 100
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100.0f;
}
