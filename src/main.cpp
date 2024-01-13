#include <pthread.h>
#include <queue>
#include <SPI.h>
#include <iostream>
#include <Arduino.h>
typedef struct {
    float moisture;
    float ph;
    std::string nodeId;
} data;

std::queue<data> globalQueue;
pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;

float getMoisture();
float getPh();

void* listen(void*);
void* send(void*);
void* capture(void*);

void setup()
{
    Serial.begin(115200);

    pthread_t th_listen;
    pthread_t th_capture;
    pthread_t th_send;

    delay(2000);

    pthread_create(&th_listen, NULL, listen, NULL);
    pthread_create(&th_capture, NULL, capture, NULL);
    pthread_create(&th_send, NULL, send, NULL);

    pthread_join(th_listen, NULL);
    pthread_join(th_capture, NULL);
    pthread_join(th_send, NULL);
}

void loop() {}

void* listen(void*)
{
    data sensorData;
    sensorData.nodeId = "7D2322";

    while (1) 
    {
        sensorData.moisture = getMoisture();
        sensorData.ph = getPh();

        printf("Listening Moisture: %.2f, pH: %.2f, nodeId: %s\n", sensorData.moisture, sensorData.ph, sensorData.nodeId.c_str());

        pthread_mutex_lock(&queueMutex);
        globalQueue.push(sensorData);
        pthread_mutex_unlock(&queueMutex);
        delay(60000);
    }
}




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
    data recData;
    while (1) 
    {
        pthread_mutex_lock(&queueMutex);
        if (!globalQueue.empty()) 
        {
            recData = globalQueue.front();
            // Convert the nodeId to a String before concatenation
            // String message = "Peeked at element in the queue, sending via lora: " + String(recData.nodeId.c_str());
            // Serial.println(message);
            Serial.println("Sending lora packet through LORA module");
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


