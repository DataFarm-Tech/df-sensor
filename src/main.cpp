#include "sensorState.h"

SensorState currentState = SensorState::BOOTING

void setup()
{
    Serial.begin(115200);
}

void loop() 
{
    switch (currentState) 
    {
        case BridgeState::BOOTING:
            //do something then change to new state
            break;
        
        case BridgeState::RECIEVING:
            //do something then change to new state
            break;

        case BridgeState::RELAY:
            //do something then change to new state
            break;
        case BridgeState::CAPTURE: 
        {
            //do something then change to new state            
            break;
        }
    }
    delay(1000);
}
