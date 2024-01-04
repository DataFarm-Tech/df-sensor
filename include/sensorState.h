enum class SensorState {
    BOOTING, //state when the device boots up (inital state)
    RECIEVING, //forwarding incoming messages to the next node (either sensor or gateway)
    SENDING //sending the data from the CAPTURE state as a lora message to the next node (either sensor or gateway)
};