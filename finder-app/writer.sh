#!/bin/bash

# Check if the number of arguments is 2
if [ $# -ne 2 ]; then
exit 1
fi

# Assign the arguments to variables
writefile=$1
writestr=$2

# Create the directory path if it does not exist
mkdir -p "$(dirname "$writefile")"

# Write the writestr content to writefile
echo "$writestr" > "$writefile"

# Check if the file creation was successful
if [ $? -eq 0 ]; then
echo "File created successfully: $writefile"
else
echo "Error: File could not be created: $writefile"
exit 1
fi
