#!/bin/bash
filename=$1
rm -f "$filename".out
gcc "$filename".c -o "$filename".out -Wall -Wextra -Werror
./"$filename".out
