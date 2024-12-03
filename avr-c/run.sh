#!/bin/sh

if [ -z "$1" ]; then
    echo "Usage: sh run.sh <TARGET>"
    exit 1
fi

TARGET=$1

make all TARGET=$TARGET
