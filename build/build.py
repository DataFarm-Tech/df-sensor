Import("env")
if env.IsIntegrationDump():
    Return()

from utils import get_id
import requests
from requests import Response
from json import dumps

node_id: str
data_to_send: dict
response: Response

url = "http://194.195.255.90:80/man/add/node" ##TODO: change to correct url
headers = {
    "Content-Type": "application/json"
}


node_id = get_id()
data_to_send = {"node_id": node_id}
print(dumps(data_to_send, indent=4))

response = requests.post(url, json=data_to_send, headers=headers, verify=False) ##verify false: allow http

if response.status_code == 200 or response.status_code == 201:
    print("OK")

# Append the configuration to the environment for the build process
env.Append(CPPDEFINES=[
    ("NODE_ID", env.StringifyMacro(node_id))
])