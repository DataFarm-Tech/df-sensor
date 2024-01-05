#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

float getPh()
{
    //read data from GPIO pins
    return ph;
}


float getMoisture()
{
    //read data from GPIO pins
    return moisture;
}


