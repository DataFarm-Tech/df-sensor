import subprocess

def write_to_eeprom(data, port="/dev/ttyUSB0", baudrate=115200):
    # Define the esptool command to write data to EEPROM
    esptool_command = [
        "esptool.py",
        "--port", port,
        "--baud", str(baudrate),
        "write_flash",
        "0x8000",  # Address of the EEPROM region in flash
        data  # Binary file containing the data to be written
    ]

    # Run the esptool command
    subprocess.run(esptool_command)

# Example usage
node_id_to_write = "info"
binary_data = node_id_to_write.encode()  # Convert string to binary data
write_to_eeprom(binary_data)
