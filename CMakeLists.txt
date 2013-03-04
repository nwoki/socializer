project(socializer)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

# Find Qt4. Need this to link and create static lib
find_package(Qt4 REQUIRED QtCore QtNetwork QtTest)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
set(CMAKE_AUTOMOC TRUE)

# sources
add_subdirectory(src)

# tests
# enable_testing()
# add_subdirectory(tests)

add_subdirectory(testApp)
