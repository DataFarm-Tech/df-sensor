from os.path import isfile

assert isfile(".env"), ".env file not found"

env_vars = {}

try:
    with open(".env", "r") as f:
        lines = f.readlines()
        for line in lines:
            key, value = line.strip().split('=', 1)
            env_vars[key] = value
except IOError:
    print("File .env not accessible")


rs485_en: int = env_vars.get("RS485_EN")
rfm95_en: int = env_vars.get("RFM95_EN")
api_host: str = env_vars.get("API_HOST")