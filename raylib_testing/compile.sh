#!/bin/bash

filename="$1"

if [ -z "$filename" ]; then
  echo "Usage: $0 <filename (without .c)>"
  exit 1
fi

# Remove old executable
rm -f "$filename"

# Compile the C file using GCC and Raylib
gcc "$filename.c" -I/usr/local/include -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o "$filename"

# Uncomment the following line for extra warnings
# -Wall -Wextra -Werror

# Run the executable
./"$filename"
