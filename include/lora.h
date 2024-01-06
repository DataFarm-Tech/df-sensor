#ifndef LORA_H
#define LORA_H


#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <SPI.h>
#include <LoRa.h>

#define ss 5
#define rst 14
#define dio0 2

void loraSend(void *pvParameters);


#endif // LORA_H