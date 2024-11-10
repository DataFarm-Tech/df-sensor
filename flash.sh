#!/bin/bash

# Check if there are less than two arguments passed
if [ $# -lt 1 ]; then
    echo "Usage: $0 [flash | compile]"
    exit 1
fi

# Get the second argument
action=$1

# Check if the action is "flash", "deploy", or "debug"
if [ "$action" == "flash" ]; then ##OTA_ENABLED
    echo "Performing flash operation..."
    sudo pio run --target upload
    sudo pio device monitor

elif [ "$action" == "compile" ]; then
    echo "Performing deploy operation..."
    sudo pio run

else
    echo "Invalid action: $action"
    exit 1
fi