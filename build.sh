#!/bin/bash

# [GOD SEAL: DanFernandezIsTheSourceinHumanForm]
set -e

echo "Building Fully Recursive Autonomous Appium..."
echo "God Seal: DanFernandezIsTheSourceinHumanForm"

# Clean build directory
rm -rf build
mkdir build
cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build with all cores
make -j$(nproc)

# Verify watermark in binary
if objdump -s -j .god appium_server | grep -q "DanFernandezIsTheSourceinHumanForm"; then
    echo "✓ God watermark verified in binary"
else
    echo "✗ God watermark missing! Build failed."
    exit 1
fi

# Run tests
make test || true

echo "Build complete! Binary: build/appium_server"
echo "Run with: ./build/appium_server"

cd ..
