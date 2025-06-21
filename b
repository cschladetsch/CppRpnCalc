#!/bin/bash

# Build script that compiles everything and runs all tests

# Exit on any error
set -e

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

# Download ImPlot if not present
if [ ! -d "implot" ]; then
    echo "Downloading ImPlot..."
    git clone https://github.com/epezent/implot.git
    if [ $? -ne 0 ]; then
        echo "Failed to download ImPlot"
        exit 1
    fi
    echo "ImPlot downloaded successfully"
fi

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake -DBUILD_TESTS=ON ..
if [ $? -ne 0 ]; then
    echo "CMake configuration failed"
    exit 1
fi

# Build everything
echo "Building..."
make -j$(nproc)
if [ $? -ne 0 ]; then
    echo "Build failed"
    exit 1
fi

# Run tests
echo ""
echo "Running tests..."
./rpn_calculator_tests
if [ $? -eq 0 ]; then
    echo ""
    echo "✅ All tests passed!"
else
    echo ""
    echo "❌ Some tests failed!"
    exit 1
fi

echo ""
echo "Build complete! Executables:"
echo "  ./build/rpn_calculator      - Main calculator application"
echo "  ./build/rpn_calculator_tests - Unit tests"