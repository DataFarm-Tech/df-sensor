#include "lora_tx_thread.h"
#include "config.h"  // For ID
#include <RH_RF95.h>  // For LoRa radio functions
#include "types.h"

lora_tx_thread* lora_tx_thread::_instance = nullptr;

lora_tx_thread::lora_tx_thread(RH_RF95& rf95) : _rf95(rf95), _taskHandle(nullptr), _isRunning(false) 
{
    // Create the mutex for synchronizing access to the queue
    _queueMutex = xSemaphoreCreateMutex();
    _instance = this;
}

void lora_tx_thread::start() 
{
    if (!_isRunning) 
    {
        _isRunning = true;
        // Create the task pinned to a core (core 1 in this case)
        xTaskCreatePinnedToCore(
            tx_thread,        // Task function
            "lora_tx_thread", // Task name
            10000,            // Stack size (bytes)
            this,             // Parameter to pass to the thread (this thread instance)
            1,                // Task priority
            &_taskHandle,     // Task handle
            1                 // Core where the task should run (core 1)
        );
        printf("[%s]: LoRa transmit task created\n", ID);
    }
}

void lora_tx_thread::stop() 
{
    if (_isRunning && _taskHandle != nullptr) 
    {
        _isRunning = false;
        vTaskDelete(_taskHandle);  // Delete the task
        _taskHandle = nullptr;
    }
}

void lora_tx_thread::tx_thread(void* parameter) 
{
    lora_tx_thread* transmit = static_cast<lora_tx_thread*>(parameter);
    
    while (transmit->_isRunning) 
    {
        // Lock the mutex before accessing the shared queue
        if (xSemaphoreTake(transmit->_queueMutex, portMAX_DELAY) == pdTRUE) 
        {
            // Check if the queue has data
            if (!upload_queue.empty()) 
            {
                // Pop the data from the queue
                data item = upload_queue.front();
                upload_queue.pop();
                printf("Queue size: %d\n", upload_queue.size());
            }
            
            // Unlock the mutex after processing
            xSemaphoreGive(transmit->_queueMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(10));  // Delay to prevent task overload
    }
    
    vTaskDelete(NULL);  // Delete the task when finished
}
