enum class SensorState {
    BOOTING, //state when the device boots up (inital state)
    RELAY, //forwarding incoming messages to the next node (either sensor or gateway)
    CAPTURE, //record the data from the GPIO pins (sensor data)
    SENDING //sending the data from the CAPTURE state as a lora message to the next node (either sensor or gateway)
};