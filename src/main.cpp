#include "sensorState.h"
void setup()
{
    Serial.begin(115200);
}

void loop() 
{
    delay(2000);
    switch () 
    {
        case BridgeState::BOOTING:
            //do something then change to new state
            break;
        
        case BridgeState::WIFI_CONNECT:
            //do something then change to new state
            break;

        case BridgeState::BLUETOOTH_CONNECT:
            //do something then change to new state
            break;
        case BridgeState::POST: 
        {
            //do something then change to new state            
            break;
        }

        case BridgeState::LORA_LISTEN:
            //do something then change to new state
            break;
    }
    delay(1000);
}
