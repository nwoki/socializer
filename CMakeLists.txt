project(socializer)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")

# Find Qt4. Need this to link and create static lib
find_package(Qt4 REQUIRED QtCore QtNetwork QtTest)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
set(CMAKE_AUTOMOC TRUE)

find_package(QJson REQUIRED)
include_directories(${QJSON_INCLUDE_DIR})

include(EnableCoverageReport)
include(EnableSlocCount)
include(GenerateCppcheck)

option(ENABLE_SOCIALIZER_EXAMPLES "Enable compilation of examples" OFF)
option(ENABLE_SOCIALIZER_TESTS "Enable compilation of tests" ON)
option(ENABLE_SOCIALIZER_COVERAGE "Enable compiler coverage" OFF)

# sources
add_subdirectory(src)

if (ENABLE_SOCIALIZER_EXAMPLES)
    add_subdirectory(testApp)
endif (ENABLE_SOCIALIZER_EXAMPLES)

if (ENABLE_SOCIALIZER_TESTS)
    enable_testing()
    #add_subdirectory(tests)
endif (ENABLE_SOCIALIZER_TESTS)

# Add these targets only if we are in the root dir
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    # --- sloccount ---

    enable_sloccount(FOLDERS src testApp)

    # --- cppcheck ---

    generate_cppcheck(SOURCES src testApp
                      ENABLE_IDS all
                      INLINE_SUPPRESSION)

    # --- coverage ---

    if (ENABLE_SOCIALIZER_COVERAGE)
        #enable_coverage_report(TARGETS Socializer
        #                       TESTS   socializer-test
        #                       FILTER "tests/* src/*")
        #add_definitions(-fprofile-arcs -ftest-coverage)
    endif (ENABLE_SOCIALIZER_COVERAGE)
endif (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
