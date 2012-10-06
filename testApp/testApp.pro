# VPATH += $${PWD}
# INCLUDEPATH += $${PWD}
# DEPENDPATH += $${PWD}

TARGET = testApp
TEMPLATE = app

QT += \
    declarative \
    network

# INCLUDEPATH += \
#     $$PWD

MOC_DIR = .moc/
OBJECTS_DIR = .obj/

# INCLUDEPATH += \
#     $${PWD}/src \
#     $${PWD}/models/objects \
#     $${PWD}/models/collections

# debug = debug symbols
# static = lib type

LIBS += -L../lib/ -lsocializer

CONFIG += \
    debug

HEADERS += \
    ../include/Foursquare \
    ../include/Facebook \
    ../include/LinkedIn \
    ../include/Twitter \
    mainwindow.h

SOURCES += \
    main.cpp \
    mainwindow.cpp
