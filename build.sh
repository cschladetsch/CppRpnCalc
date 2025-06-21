#!/bin/bash

# Download ImGui if not present
if [ ! -d "imgui" ]; then
    echo "Downloading Dear ImGui..."
    git clone https://github.com/ocornut/imgui.git
    if [ $? -ne 0 ]; then
        echo "Failed to download ImGui"
        exit 1
    fi
    echo "ImGui downloaded successfully"
fi

# Create build directory
mkdir -p build
cd build

# Configure and build
cmake ..
if [ $? -ne 0 ]; then
    echo "CMake configuration failed"
    exit 1
fi

make -j$(nproc)
if [ $? -ne 0 ]; then
    echo "Build failed"
    exit 1
fi

echo "Build complete!"
echo ""
echo "Available targets:"
echo "  ./build/rpn_calculator      - Run the calculator"
echo "  ./build/rpn_calculator_tests - Run unit tests"
echo ""

# Optionally run tests
if [ "$1" = "test" ] || [ "$1" = "--test" ]; then
    echo "Running tests..."
    ./rpn_calculator_tests
    if [ $? -eq 0 ]; then
        echo "All tests passed!"
    else
        echo "Some tests failed!"
        exit 1
    fi
fi