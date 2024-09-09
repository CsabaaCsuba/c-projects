#!/bin/bash
filename=$1
rm -f "$filename".out
gcc "$filename".c -lm -o "$filename".out -Wall -Wextra -Werror
./"$filename".out
