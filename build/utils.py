from enum import Enum

class device_type(Enum):
    CONTROLLER = "controller"
    SENSOR = "sensor"

def get_id() -> str:
    is_valid: bool = False

    while not is_valid:
        print("Enter ID: ")
        id: str = input()

        if not id.isalnum() or not len(id) == 6:
            print("Invalid ID, Please Retry.")
        else:
            is_valid = True
    return id

def get_device_type() -> device_type:
    while True:
        print("Enter device type (controller/sensor): ")
        user_input = input().strip().lower()  # Capture the input and normalize

        if user_input not in ['controller', 'sensor']:
            print("Invalid input. Please enter 'controller' or 'sensor'.")
        else:
            # Return the corresponding device_type enum member
            return device_type[user_input.upper()]
