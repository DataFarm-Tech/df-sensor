Import("env")
if env.IsIntegrationDump():
    Return()

from utils import get_id
import requests
from requests import Response
from json import dumps
from constants import *

node_id: str
data_to_send: dict
response: Response

headers: dict[str, str] = {
    "Content-Type": "application/json"
}

node_id = get_id()
data_to_send = {"node_id": node_id}
print(dumps(data_to_send, indent=4))

response = requests.post(api_host, json=data_to_send, headers=headers, verify=False) ##verify false: allow http

if response.status_code == 200 or response.status_code == 201:
    print("OK")

# Append the configuration to the environment for the build process
env.Append(CPPDEFINES=[
    ("NODE_ID", env.StringifyMacro(node_id)),
    ("RFM95_EN", env.StringifyMacro(rfm95_en)),
    ("RS485_EN", env.StringifyMacro(rs485_en))
])