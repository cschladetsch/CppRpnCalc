#!/bin/bash

# Quick test runner script
echo "Building and running tests..."

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

# Configure and build with tests
cmake ..
if [ $? -ne 0 ]; then
    echo "CMake configuration failed"
    exit 1
fi

make rpn_calculator_tests -j$(nproc)
if [ $? -ne 0 ]; then
    echo "Test build failed"
    exit 1
fi

# Run tests
echo ""
echo "Running unit tests..."
./rpn_calculator_tests

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ All tests passed!"
else
    echo ""
    echo "❌ Some tests failed!"
    exit 1
fi