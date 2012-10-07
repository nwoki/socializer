TARGET = testApp
TEMPLATE = app

QT += \
    declarative \
    network

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
    ../include/Twitter

SOURCES += \
    main.cpp
