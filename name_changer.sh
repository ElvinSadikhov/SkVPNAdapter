#!/bin/bash

# Check if three arguments are provided
if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <target_word> <replacement_word> <directory>"
    exit 1
fi

# Input parameters
TARGET_WORD="$1"
REPLACEMENT_WORD="$2"
DIRECTORY="$3"

# Function to replace content inside files
replace_content() {
    local file="$1"
    if grep -q "$TARGET_WORD" "$file"; then
        if [[ "$OSTYPE" == "darwin"* ]]; then
            # For macOS
            sed -i '' "s/${TARGET_WORD}/${REPLACEMENT_WORD}/g" "$file"
        else
            # For Linux
            sed -i "s/${TARGET_WORD}/${REPLACEMENT_WORD}/g" "$file"
        fi
        echo "Replaced content in: $file"
    fi
}

# Function to rename files and directories
rename_files_and_directories() {
    for item in "$1"/*; do
        if [ -d "$item" ]; then
            # Recursively handle directories
            rename_files_and_directories "$item"

            # Rename the directory if it contains the target word
            dir_name=$(basename "$item")
            new_dir_name=$(echo "$dir_name" | sed "s/${TARGET_WORD}/${REPLACEMENT_WORD}/g")
            if [ "$dir_name" != "$new_dir_name" ]; then
                mv "$item" "$(dirname "$item")/$new_dir_name"
                echo "Renamed directory: $dir_name -> $new_dir_name"
            fi
        elif [ -f "$item" ]; then
            # Replace content inside the file
            replace_content "$item"

            # Rename the file if it contains the target word
            file_name=$(basename "$item")
            new_file_name=$(echo "$file_name" | sed "s/${TARGET_WORD}/${REPLACEMENT_WORD}/g")
            if [ "$file_name" != "$new_file_name" ]; then
                mv "$item" "$(dirname "$item")/$new_file_name"
                echo "Renamed file: $file_name -> $new_file_name"
            fi
        fi
    done
}

# Start the process
rename_files_and_directories "$DIRECTORY"

echo "Replacement process completed."

