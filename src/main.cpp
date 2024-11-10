#include <Arduino.h>
#include "config.h"

void setup(void);
void loop(void);

void setup(void)
{
    Serial.begin(115200);
    printf("[%c] init serial console\n", ID);
    printf("[%c] init lora comms\n", ID);
}

void loop(void)
{
    // TODO
}
