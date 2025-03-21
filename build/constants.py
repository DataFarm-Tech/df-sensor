from os.path import isfile

assert isfile(".env"), ".env file not found"

env_vars = {}

try:
    with open(".env", "r") as f:
        lines = f.readlines()
        for line in lines:
            # Ignore empty lines and lines that do not contain an '=' character
            if '=' in line:
                key, value = line.strip().split('=', 1)
                env_vars[key] = value
            else:
                print(f"Skipping invalid line in .env file: {line.strip()}")
except IOError:
    print("File .env not accessible")

rs485_en: str = int(env_vars.get("RS485_EN", 0))  # Default to 0 if not found
rfm95_en: str = int(env_vars.get("RFM95_EN", 0))  # Default to 0 if not found
api_host: str = env_vars.get("API_HOST", "")  # Default to empty string if not found
