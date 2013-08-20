project(socializer)

cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/common-modules/" "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")

find_package(Qt5 COMPONENTS Core Network REQUIRED)
find_package(CommonModules 3 REQUIRED)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

include(EnableCoverageReport)
include(EnableSlocCount)
include(GenerateCppcheck)

option(ENABLE_SOCIALIZER_EXAMPLES "Enable compilation of examples" ON)
option(ENABLE_SOCIALIZER_TESTS "Enable compilation of tests" OFF)
option(ENABLE_SOCIALIZER_COVERAGE "Enable compiler coverage" OFF)

# C++11
if (CMAKE_COMPILER_IS_GNUCXX)
   execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
   if (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
        message(STATUS "C++11 activated.")
        add_definitions("-std=gnu++11")
   elseif (GCC_VERSION VERSION_GREATER 4.3 OR GCC_VERSION VERSION_EQUAL 4.3)
        message(WARNING "C++0x activated. If you get any errors update to a compiler which fully supports C++11")
        add_definitions("-std=gnu++0x")
   else ()
        message(FATAL_ERROR "C++11 needed. Therefore a gcc compiler with a version higher than 4.3 is needed.")
   endif()
else (CMAKE_COMPILER_IS_GNUCXX)
   add_definitions("-std=c++0x")
endif (CMAKE_COMPILER_IS_GNUCXX)

# sources
add_subdirectory(src)

if (ENABLE_SOCIALIZER_EXAMPLES)
    add_subdirectory(examples)
endif (ENABLE_SOCIALIZER_EXAMPLES)

if (ENABLE_SOCIALIZER_TESTS)
    enable_testing()
    #add_subdirectory(tests)
endif (ENABLE_SOCIALIZER_TESTS)

# # Add these targets only if we are in the root dir
# if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
#     # --- sloccount ---
# 
#     enable_sloccount(FOLDERS src testApp)
# 
#     # --- cppcheck ---
# 
#     generate_cppcheck(SOURCES src testApp
#                       ENABLE_IDS all
#                       INLINE_SUPPRESSION)
# 
#     # --- coverage ---
# 
#     if (ENABLE_SOCIALIZER_COVERAGE)
#         #enable_coverage_report(TARGETS Socializer
#         #                       TESTS   socializer-test
#         #                       FILTER "tests/* src/*")
#         #add_definitions(-fprofile-arcs -ftest-coverage)
#     endif (ENABLE_SOCIALIZER_COVERAGE)
# endif (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
