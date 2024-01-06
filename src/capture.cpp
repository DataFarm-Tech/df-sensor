#include "capture.h"
#include "eeprom.h"

// Declare the queue handle defined in main.cpp
extern QueueHandle_t xQueue;

void capture(void *pvParameters) 
{
  data sensorData;

  eeprom();
  eeprom::restoreEEPROM();
  sensorData.nodeId = eeprom::getNodeId();

  while (1) 
  {
    // Simulate reading data from GPIO pins
    sensorData.moisture = getMoisture();
    sensorData.ph = getPh();

    printf("Sending Moisture: %.2f, pH: %.2f, nodeId: %s\n", sensorData.moisture, sensorData.ph, sensorData.nodeId.c_str());
    
    // Send the value to the queue
    xQueueSend(xQueue, &sensorData, portMAX_DELAY);
    
    // Delay for a while before sending the next value
    vTaskDelay(pdMS_TO_TICKS(60000));
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
