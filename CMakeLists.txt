project(socializer)

cmake_minimum_required(VERSION 2.8.9)

option(QT5_BUILD "Enable compilation with Qt5" FALSE)
option(ENABLE_DYNAMIC "Enable dynamic library build" FALSE)
option(ENABLE_SOCIALIZER_EXAMPLES "Enable compilation of examples" ON)
option(ENABLE_SOCIALIZER_TESTS "Enable compilation of tests" OFF)
option(ENABLE_SOCIALIZER_COVERAGE "Enable compiler coverage" OFF)
option(ENABLE_WERROR "Enables WError. Always enable when developing, and disable when releasing." ON)

# Qt version
if (QT5_BUILD)
    message(STATUS "Building for Qt5")
    find_package(Qt5 COMPONENTS Core Network REQUIRED)
    add_definitions(-DUSING_QT5)
else ()
    message(STATUS "Building for Qt4")
    find_package(Qt4 REQUIRED QtCore QtNetwork QtXml)
    include(${QT_USE_FILE})
    add_definitions(${QT_DEFINITIONS})
endif(QT5_BUILD)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# Library version
set(SOCIALIZER_ABI_VERSION 0)
set(SOCIALIZER_MINOR_VERSION 1)
set(SOCIALIZER_RELEASE_VERSION 0)

# include(EnableCoverageReport)
# include(EnableSlocCount)
# include(GenerateCppcheck)

# Warnings. Enable WError, this component is critical.
if(${ENABLE_WERROR})
    set(RET_W 1)
else(${ENABLE_WERROR})
    set(RET_W 0)
endif(${ENABLE_WERROR})

# VERSIONING
set(SOCIALIZER_VERSION ${SOCIALIZER_ABI_VERSION}.${SOCIALIZER_MINOR_VERSION}.${SOCIALIZER_RELEASE_VERSION})

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


# build type
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Build type not specified. Defaulting to DEBUG")
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL Release)
    message(STATUS "Building RELEASE version")
    set(CMAKE_CSS_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL Debug)
    message(STATUS "Building DEBUG version")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
endif()


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
