#include <Arduino.h>
#include "lora_rx_thread.h"
#include "config.h"
#include "utils.h"
#include "rs485_int.h"

TaskHandle_t lora_listener_th; // thread handler for lora listen thread
TaskHandle_t rs485_poll_th; // thread handler for rs485 polling thread

/*
init lora config and rs485 setup
*/
void setup()
{
    sleep(2);
    Serial.begin(BAUD_RATE);
    PRINT_STR("staring serial console");
    init_lora();
    init_rs485();

    xTaskCreatePinnedToCore(lora_listener, "lora_listener", 10000, NULL, 1, &lora_listener_th, 0); // create lora listen thread
    xTaskCreatePinnedToCore(rs485_poll, "rs485_poll", 10000, NULL, 1, &rs485_poll_th, 1); // create lora listen thread
}

/*
Infinite Loop
*/
void loop()
{
    delay(5000);
}