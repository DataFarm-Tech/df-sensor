#include "lora.h"
#include "capture.h"

// Define the queue handle
QueueHandle_t xQueue;

void setup() 
{
  // Create a queue to hold 5 values of type data
  xQueue = xQueueCreate(5, sizeof(data));

  // Create tasks
  xTaskCreate(capture, "capture", 3000, NULL, 1, NULL);
  xTaskCreate(loraSend, "loraSend", 3000, NULL, 2, NULL);
}

void loop() 
{
  // Empty loop as we are using FreeRTOS tasks
}
