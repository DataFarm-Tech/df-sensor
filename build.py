# pre_build.py
import requests
# print("Enter NODEID: ")
# nodeId = input()
nodeId = "7DUJHHD"
url = "https://9527-220-245-234-225.ngrok-free.app/node/add/man"
api_key = "8151D74D23320A035F6DC0717F54B978BE3A29100AE52E716170D8A84B939BAC5C14EE537D1174919F67BA916E29D4DB696E73B4296E5F6FD2BE16984B1D31F1"

data = {"nodeId": nodeId}
headers = {
    "Content-Type": "application/json",
    "X-API-Key": api_key
}
response = requests.post(url, json=data, headers=headers)
print(response)
Import("env")
env.Append(CPPDEFINES=[
    ("NODE_ID", env.StringifyMacro(nodeId)),
])