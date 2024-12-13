#ifndef TYPES_H
#define TYPES_H

#include <SPI.h>
#include <RH_RF95.h>

enum class controller_state
{
    BOOTING,
    WIFI_CONNECT,
    SLEEP
};

typedef struct {
    int humidity;
    int temperature;
    int conductivity;
    int ph;
    int nitrogen;
    int phosphorus;
    int potassium;
    String nodeId;  // Kept the nodeId from old structure
} data;

typedef struct {
    String bridgeId;
    float sw_ver;
} sw_ver_update;

#endif  // TYPES_H