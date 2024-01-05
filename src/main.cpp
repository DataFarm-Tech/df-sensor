#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

void setup()
{
    Serial.begin(115200);
    
    counterMutex = xSemaphoreCreateMutex();
    if (counterMutex != NULL) 
    {
        xTaskCreate(incrementTask,"IncrementTask",10000,NULL,1,NULL);
        xTaskCreate(decrementTask,"DecrementTask",10000,NULL,2,NULL);
        vTaskStartScheduler();
    } 
    else 
    {
        Serial.println("Mutex creation failed!");
    }
}


void loop()
{

}