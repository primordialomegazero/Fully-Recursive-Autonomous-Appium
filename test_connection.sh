#!/bin/bash
echo "Testing Appium connection..."
curl -X GET http://localhost:6790/wd/hub/status 2>/dev/null || echo "Server not ready yet"
