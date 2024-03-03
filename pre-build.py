# Ask the user for a nodeId
node_id = input("Enter the nodeId: ")

# Write the nodeId to a file
filename = "nodeId.txt"
with open(filename, "w") as file:
    file.write(node_id)

print(f"Node ID '{node_id}' has been written to '{filename}'.")
