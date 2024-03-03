# pre_build.py
import requests

# Read nodeId from the file
with open("nodeId.txt", "r") as file:
    nodeId = file.read().strip()

# Define the data dictionary with static values
data = {
    "nodeId": nodeId
}

# API endpoint and API key
url = "http://45.79.239.100:8000/node/add/man"
api_key = "8151D74D23320A035F6DC0717F54B978BE3A29100AE52E716170D8A84B939BAC5C14EE537D1174919F67BA916E29D4DB696E73B4296E5F6FD2BE16984B1D31F1"

# POST request with data and headers
headers = {
    "Content-Type": "application/json",
    "X-API-Key": api_key
}
response = requests.post(url, json=data, headers=headers)
print(response)

# Define NODE_ID macro for PlatformIO
Import("env")
env.Append(CPPDEFINES=[
    ("NODE_ID", env.StringifyMacro(nodeId)),
])
