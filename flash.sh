#!/bin/bash

# Function to display help text
function display_help {
    echo "Usage: $0 [flash | compile | clean]"
    echo
    echo "This script automates operations for building and flashing firmware on datafarm devices using PlatformIO."
    echo
    echo "Arguments:"
    echo "  flash    : Performs the flash operation to upload the firmware to the ESP32 device."
    echo "  compile  : Compiles the project without uploading to the device."
    echo "  clean    : Cleans the build environment (removes temporary and build files)."
    echo
    echo "Examples:"
    echo "  $0 flash   : Flash the firmware to the ESP32."
    echo "  $0 compile : Compile the project without uploading."
    echo "  $0 clean   : Clean the build environment."
    echo
    echo "If no argument is provided, or more than one, the script will exit with a usage message."
    echo "Note: To configure the upload_port, baud_rate, and libs, modify the platformio.ini file."
    exit 1
}

# Check if the number of arguments is exactly 1
if [ $# -ne 1 ]; then
    display_help
fi

# Get the first argument
action=$1

# Function to check if the port exists
function check_port_existence {
    if [ ! -e "$1" ]; then
        echo "Error: Serial port $1 not found!"
        echo "Make sure the device is connected and try again."
        exit 1
    fi
}

# Prompt for port if action is flash
if [ "$action" == "flash" ]; then
    read -p "Enter the serial port (e.g., /dev/ttyUSB0): " port
    if [ -z "$port" ]; then
        echo "Error: No port specified!"
        exit 1
    fi

    check_port_existence "$port"

    echo "Performing flash operation on port $port..."
    sudo pio run -e flash --target upload --upload-port "$port"
    sudo pio device monitor -b 115200 -p "$port"

elif [ "$action" == "compile" ]; then
    echo "Performing compile operation..."
    sudo pio run -e compile

elif [ "$action" == "clean" ]; then
    echo "Cleaning build environment..."
    sudo pio run -e compile --target clean

else
    echo "Invalid action: $action"
    display_help
fi
