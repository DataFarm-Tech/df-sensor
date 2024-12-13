#ifndef LORA_TX_THREAD_H
#define LORA_TX_THREAD_H

#include <RH_RF95.h>
#include "types.h"  // For the 'data' type
#include <queue>

extern std::queue<data> upload_queue;  // Shared queue between threads

class lora_tx_thread {
public:
    lora_tx_thread(RH_RF95& rf95); // Constructor with reference to RH_RF95 object
    void start();                  // Start the LoRa transmit thread
    void stop();                   // Stop the LoRa transmit thread
    SemaphoreHandle_t getQueueMutex() { return _queueMutex; }  // Getter for the mutex

private:
    static void tx_thread(void* parameter);  // Static function for the LoRa TX task

    RH_RF95& _rf95;        // Reference to the LoRa radio object
    TaskHandle_t _taskHandle;  // Task handle for the TX task
    SemaphoreHandle_t _queueMutex;  // Mutex for synchronizing queue access
    bool _isRunning;      // Flag to track if the thread is running

    static lora_tx_thread* _instance;  // Singleton instance of this class
};

#endif // LORA_TX_THREAD_H
