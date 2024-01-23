import re

def update_node_id(node_id):
    # Read the contents of the platformio.ini file
    with open("platformio.ini", "r") as file:
        config_lines = file.readlines()

    # Find and update the line containing NODE_ID in build_flags
    node_id_pattern = re.compile(r"build_flags\s*=\s*-D\s+NODE_ID=(\w+)")
    for i, line in enumerate(config_lines):
        match = node_id_pattern.search(line)
        if match:
            # Replace the existing NODE_ID with the new value
            config_lines[i] = f"build_flags = -D NODE_ID={node_id}\n"
            break
    else:
        # If no existing NODE_ID is found, add a new build_flags line
        config_lines.append(f"build_flags = -D NODE_ID={node_id}\n")

    # Write the modified contents back to the platformio.ini file
    with open("platformio.ini", "w") as file:
        file.writelines(config_lines)

    # Print a message for confirmation
    print(f"Updated NODE_ID in platformio.ini to {node_id}")

# Prompt the user for the new NODE_ID
new_node_id = input("Enter the new NODE_ID (Digits only allowed): ")

# Call the function to update the NODE_ID in platformio.ini
update_node_id(new_node_id)
