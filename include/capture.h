#ifndef CAPTURE_H
#define CAPTURE_H

#include <Arduino.h>

typedef struct {
    float moisture;
    float ph;
    std::string nodeId;
} data;

float getPh();
float getMoisture();
void capture(void *pvParameters);

#endif // CAPTURE_H
