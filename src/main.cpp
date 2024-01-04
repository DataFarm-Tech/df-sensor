#include "sensorState.h"

SensorState currentState = SensorState::BOOTING;

void setup() {
    Serial.begin(115200);
}

void loop() {
    switch (currentState) {
        case SensorState::BOOTING:
            // Perform actions for BOOTING state
            Serial.println("Booting up...");
            
            // Transition to the next state (e.g., CAPTURE)
            currentState = SensorState::CAPTURE;
            break;

        case SensorState::CAPTURE:
            // Perform actions for CAPTURE state
            Serial.println("Capturing sensor data...");
            
            // Transition to the next state (e.g., SENDING)
            currentState = SensorState::SENDING;
            break;

        case SensorState::SENDING:
            // Perform actions for SENDING state
            Serial.println("Sending sensor data...");
            
            // Transition to the next state (e.g., RELAY or BOOTING)
            currentState = SensorState::RELAY;
            break;

        case SensorState::RELAY:
            // Perform actions for RELAY state
            Serial.println("Relaying messages...");
            
            // Transition to the next state (e.g., BOOTING or CAPTURE)
            currentState = SensorState::BOOTING;
            break;
    }

    delay(1000);
}
