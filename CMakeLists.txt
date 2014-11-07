set(CMAKE_TOOLCHAIN_FILE "cmake/msp430.cmake")

project(RF24Sender)
cmake_minimum_required(VERSION 2.8)

# Configuration
# =============
#
set(CONF_FREQUENCY_IN_HZ "16000000UL" CACHE STRING "CPU frequency used")

# RF2500 UPLOAD
add_custom_target(upload
  COMMAND mspdebug rf2500 "prog RF24Sender"
  DEPENDS RF24Sender
  COMMENT "Uploading RF24Sender")

# Compiler Settings
add_definitions("-DF_CPU=${CONF_FREQUENCY_IN_HZ}")

# MSP430Lib
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/msp430lib/include
)
add_subdirectory(msp430lib)

# RF24
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/RF24/include
)
add_subdirectory(RF24)

add_executable(
  RF24Sender
    main.cpp
)
target_link_libraries(
  RF24Sender
    msp430lib
    RF24_MSP430
)
