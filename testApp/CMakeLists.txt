project(testApp)

cmake_minimum_required(VERSION 2.6)

# Find Qt4. Need this to link and create static lib
FIND_PACKAGE(Qt4 REQUIRED)

set(QT_USE_QTCORE TRUE)
set(QT_USE_QTNETWORK TRUE)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

include_directories(${QT_INCLUDE_DIR})

set(testApp_SRCS
    main.cpp
)

# qt4_wrap_cpp(testApp_MOC_HDRS ${testApp_HDRS})
# qt4_wrap_ui(testApp_FORMS_HDRS ${testApp_FORMS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# final app
add_executable(testApp ${testApp_SRCS} ${testApp_MOC_HDRS} ${testApp_FORMS_HDRS})

# link app with Qt libs
target_link_libraries(testApp ${QT_LIBRARIES} Socializer)
