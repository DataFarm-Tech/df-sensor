#include <Arduino.h>
#include "lora_rx_thread.h"
#include "config.h"
#include "sensor_int.h"

TaskHandle_t lora_listener_th; // thread handler for lora listen thread
TaskHandle_t rs485_poll_th; // thread handler for lora listen thread

/*
init lora config and rs485 setup
*/
void setup()
{
    sleep(2);
    Serial.begin(BAUD_RATE);
    setup_lora();
    init_rs485();

    printf("[%s]: starting serial console\n", NODE_ID);
    printf("[%s]: init lora listen thread\n", NODE_ID);
    // xTaskCreatePinnedToCore(lora_listener, "lora_listener", 10000, NULL, 1, &lora_listener_th, 0); // create lora listen thread
    xTaskCreatePinnedToCore(rs485_poll, "rs485_poll", 10000, NULL, 1, &rs485_poll_th, 0); // create lora listen thread
}

/*
Infinite Loop
*/
void loop()
{
    uint8_t lora_data_rx[19];
    int res = read_sensor(lora_data_rx);
    if (res == 0)
        printf("Sensor reading worked\n");

    delay(5000);
}