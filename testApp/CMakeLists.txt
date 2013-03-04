# Find Qt4. Need this to link and create static lib
FIND_PACKAGE(Qt4 REQUIRED QtCore QtNetwork QtDeclarative QtGui)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

include_directories(${QT_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(testApp_SRCS
    main.cpp
)

# final app
add_executable(testApp ${testApp_SRCS})

# link app with Qt libs
target_link_libraries(testApp ${QT_QTGUI_LIBRARY} ${QT_QTDECLARATIVE_LIBRARY} Socializer)
