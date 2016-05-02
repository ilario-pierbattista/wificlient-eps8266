cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME wificlient_eps8266)
project(${PROJECT_NAME})

# Target di compilazione (identificatore della scheda)
set(${CMAKE_PROJECT_NAME}_BOARD uno)
# Porta seriale alla quale è collegata la scheda
set(${CMAKE_PROJECT_NAME}_PORT /dev/ttyACM0)

set(${CMAKE_PROJECT_NAME}_SKETCH wificlient_eps8266.ino)
generate_arduino_firmware(${CMAKE_PROJECT_NAME})
