#include <Arduino.h>
#include "lora_rx_thread.h"
#include "config.h"
#include "sensor_int.h"


TaskHandle_t lora_listener_th; //thread handler for lora listen thread

void setup()
{
    sleep(2);
    Serial.begin(BAUD_RATE);
    setup_lora();
    init_rs485();

    printf("[%s]: starting serial console\n", NODE_ID);
    printf("[%s]: init lora listen thread\n", NODE_ID);
    xTaskCreatePinnedToCore(lora_listener, "lora_listener", 10000, NULL, 1, &lora_listener_th, 0); //create lora listen thread
}

void loop()
{
    uint8_t rs485_data[13];
    read_sensor(rs485_data);

    Serial.printf("[%s] Humidity: %d\n", NODE_ID, rs485_data[0]);
    Serial.printf("[%s] Temperature: %d\n", NODE_ID, rs485_data[1]);
    Serial.printf("[%s] Conductivity: %d\n", NODE_ID, rs485_data[2]);
    Serial.printf("[%s] pH: %d\n", NODE_ID, rs485_data[3]);
    Serial.printf("[%s] Nitrogen: %d\n", NODE_ID, rs485_data[4]);
    Serial.printf("[%s] Phosphorus: %d\n", NODE_ID, rs485_data[5]);
    Serial.printf("[%s] Potassium: %d\n", NODE_ID, rs485_data[6]);

    delay(5000);

}