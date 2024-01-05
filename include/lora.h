#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

void send(void *pvParameters, String nodeId, float ph, float moisture);
void listen(void *pvParameters) //not sure what param needed for this function yet