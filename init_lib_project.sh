#!/bin/bash

if ! command -v python3 &> /dev/null
then
    echo "Python3 isn't installed. Please install it and try again."
    exit 1
fi

python3 init_lib_project.py "$@"