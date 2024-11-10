import requests
import uuid
import json
from utils import get_id, get_device_type, device_type
from enum import Enum

Import("env")

headers = {
    "Content-Type": "application/json"
}

device: device_type = get_device_type()
id: str = get_id()
api_key = None

if device == device_type.CONTROLLER:
    api_key: str = uuid.uuid4()
    print("Controller", id)
    url = "http://194.195.255.90:80/man/add/controller"
    data = {"controller_id": id, "api_key": str(api_key)}
    response = requests.post(url, json=data, headers=headers)

    env.Append(CPPDEFINES=[
    ("ID", env.StringifyMacro(id)),
    ("OTA_ENABLED", 0),  # OTA_ENABLED is set to false
    ("API_KEY", env.StringifyMacro(str(api_key)))])

else:
    print("Node", id)
    url = "http://194.195.255.90:80/man/add/node"
    data = {"nodeId": id}
    response = requests.post(url, json=data, headers=headers)
    
    env.Append(CPPDEFINES=[("ID", env.StringifyMacro(id))])

if response.status_code != 201:
    print(json.dumps(data, indent=4))
    exit()