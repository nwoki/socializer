project(socializer)

cmake_minimum_required(VERSION 2.6)

# Find Qt4. Need this to link and create static lib
FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

include(${QT_USE_FILE})

# debug symbols
add_definitions(-g -DDEBUG_MODE)
add_definitions(${QT_DEFINITIONS})

include_directories(${QT_INCLUDE_DIR})

# sources
add_subdirectory(src)

# tests
# enable_testing()
# add_subdirectory(tests)

add_subdirectory(testApp)