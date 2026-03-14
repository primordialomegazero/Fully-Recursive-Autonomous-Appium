cmake_minimum_required(VERSION 3.10)
project(FullyRecursiveAutonomousAppium)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/core/AdaptiveServer.cpp
    src/system/SystemDetector.cpp
    src/system/MemoryLimiter.cpp
    src/god/watermark.cpp
    src/drivers/AndroidDriver.cpp
    src/drivers/IOSDriver.cpp
    src/security/QuantumSecurity.cpp
    src/w3c/WebDriverProtocol.cpp
    src/monitoring/MetricsServer.cpp
    src/ai/NanoAgent.cpp
    src/ai/AgentSwarm.cpp
    src/ai/RecursiveOrchestrator.cpp
    src/control/LyapunovStability.cpp
    src/control/StabilityMonitor.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

find_library(LIBOQS_LIBRARY oqs)
if(LIBOQS_LIBRARY)
    message(STATUS "Found liboqs")
    include_directories(/usr/local/include)
endif()

add_executable(appium_recursive ${SOURCES})

target_link_libraries(appium_recursive
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENSSL_LIBRARIES}
    ${LIBOQS_LIBRARY}
    pthread
    dl
)

target_compile_options(appium_recursive PRIVATE -O2 -Wall -Wno-unused-parameter)

# Compile swagger server separately
add_executable(swagger_server swagger_server.cpp)
target_link_libraries(swagger_server pthread)
