#!/bin/bash

# Run script that builds everything using 'b' and then runs the main executable

# Exit on any error
set -e

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Run the build script
echo "Building project..."
"${SCRIPT_DIR}/b"
if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

# Run the main executable
echo ""
echo "Running RPN Calculator..."
cd "${SCRIPT_DIR}"
./build/rpn_calculator