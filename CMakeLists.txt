project(socializer)

cmake_minimum_required(VERSION 2.6)

# Find Qt4. Need this to link and create static lib
FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

set(QT_USE_QTCORE TRUE)
set(QT_USE_QTNETWORK TRUE)
set(QT_DONT_USE_QTGUI TRUE)

include(${QT_USE_FILE})

# add_definitions(${QT_DEFINITIONS})
# add_definitions(-DSTATIC)

include_directories(${QT_INCLUDE_DIR})

# sources
add_subdirectory(src/)

# tests
# enable_testing()
# add_subdirectory(tests)
