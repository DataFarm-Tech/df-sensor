Import("env")
# This fixes the infinite loop problem when flashing the ESP32 with the Arduino framework on Windows
if env.IsIntegrationDump():
    Return()

import requests
import uuid
import json
from utils import get_id, get_device_type, device_type
from enum import Enum

headers = {
    "Content-Type": "application/json"
}

ssid: str = ""
password: str = ""
api_key: str = ""

device: device_type = get_device_type()
id: str = get_id()
ota_enabled: int = 0  # Initially, OTA is disabled

if device == device_type.CONTROLLER:
    url = "https://194.195.255.90:443/man/add/controller"
    api_key = uuid.uuid4()
    data = {"controller_id": id, "api_key": str(api_key)}
    response = requests.post(url, json=data, headers=headers, verify=False)
    
    # Ask the user if they would like to enable OTA
    print("Would you like to enable OTA? (yes/no): ")
    ota_choice = input().strip().lower()
    
    if ota_choice == "yes":
        ota_enabled = 1  # Enable OTA
        print("OTA enabled.")
    else:
        print("OTA not enabled.")
    
    print("Enter SSID: ")
    ssid: str = input()
    print("Enter Password: ")
    password: str = input()

elif device == device_type.SENSOR:
    url = "https://194.195.255.90:443/man/add/node"
    data = {"nodeId": id}
    response = requests.post(url, json=data, headers=headers, verify=False)

# Print the data being sent for debugging purposes
print(json.dumps(data, indent=4))

if response.status_code == 409:
    print(f"{id} already exists.")
    exit()

# Append the configuration to the environment for the build process
env.Append(CPPDEFINES=[
    ("ID", env.StringifyMacro(id)),
    ("SSID_CRED", env.StringifyMacro(ssid)),
    ("PASS_CRED", env.StringifyMacro(password)),
    ("OTA_ENABLED", ota_enabled),  # OTA_ENABLED is set based on user's choice
    ("API_KEY", env.StringifyMacro(str(api_key)))
])
