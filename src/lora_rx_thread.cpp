#include "lora_rx_thread.h"
#include "config.h"  // For ID
#include <RH_RF95.h>  // For LoRa radio functions
#include "types.h"

lora_rx_thread* lora_rx_thread::_instance = nullptr;

lora_rx_thread::lora_rx_thread(RH_RF95& rf95): _rf95(rf95), _taskHandle(nullptr), _isRunning(false) 
{
    // Create the mutex for synchronizing access to the queue
    _queueMutex = xSemaphoreCreateMutex();
    _instance = this;   
}

void lora_rx_thread::start() 
{
    if (!_isRunning) 
    {
        _isRunning = true;
        // Create the task pinned to a core (core 0 in this case)
        xTaskCreatePinnedToCore(
            rx_thread,        // Task function
            "lora_rx_thread", // Task name
            10000,            // Stack size (bytes)
            this,             // Parameter to pass to the thread (this thread instance)
            1,                // Task priority
            &_taskHandle,     // Task handle
            0                 // Core where the task should run (core 0)
        );
        printf("[%s]: LoRa listener task created\n", ID);
    }
}

void lora_rx_thread::stop() 
{
    if (_isRunning && _taskHandle != nullptr) 
    {
        _isRunning = false;
        vTaskDelete(_taskHandle);  // Delete the task
        _taskHandle = nullptr;
    }
}

void lora_rx_thread::rx_thread(void* parameter) 
{
    lora_rx_thread* listener = static_cast<lora_rx_thread*>(parameter);
    
    while (listener->_isRunning) 
    {
        // Check if data is available
        if (listener->_rf95.available()) 
        {
            uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
            uint8_t len = sizeof(buf);
            
            // Receive the message
            if (listener->_rf95.recv(buf, &len)) 
            {
                printf("msg rec\n");

                // Lock the mutex before modifying the shared queue
                if (xSemaphoreTake(listener->_queueMutex, portMAX_DELAY) == pdTRUE) 
                {
                    data receivedData;
                    receivedData.humidity = 2;
                    receivedData.temperature = 3;
                    receivedData.conductivity = 9;
                    receivedData.ph = 13;
                    receivedData.nitrogen = 98;
                    receivedData.phosphorus = 98;
                    receivedData.potassium = 22;
                    receivedData.nodeId = "Hello";


                    upload_queue.push(receivedData);
                    // Unlock the mutex after updating the shared resource
                    xSemaphoreGive(listener->_queueMutex);
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));  // Delay to prevent task overload
    }
    
    vTaskDelete(NULL);  // Delete the task when finished
}
