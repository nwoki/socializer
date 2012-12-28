# VPATH += $${PWD}
# INCLUDEPATH += $${PWD}
# DEPENDPATH += $${PWD}

TARGET = socializer
TEMPLATE = lib

QT += \
    declarative \
    network

MOC_DIR = .moc/
OBJECTS_DIR = .obj/
DESTDIR = ../lib/

# INCLUDEPATH += \
#     $${PWD}/src \
#     $${PWD}/models/objects \
#     $${PWD}/models/collections

# debug = debug symbols
# static = lib type
CONFIG += \
    debug \
    static

HEADERS += \
    ../include/Foursquare \
    ../include/Facebook \
    ../include/LinkedIn \
    ../include/Twitter \
    facebook.h \
    foursquare.h \
    linkedin.h \
    oauth.h \
    twitter.h \
    # QJSON PART
    qjson/src/parser.h \
    qjson/src/qobjecthelper.h \
    qjson/src/json_scanner.h \
    qjson/src/json_parser.hh \
    qjson/src/parserrunnable.h \
    qjson/src/serializer.h \
    qjson/src/serializerrunnable.h \

SOURCES += \
    facebook.cpp \
    foursquare.cpp \
    linkedin.cpp \
    oauth.cpp \
    twitter.cpp \
        # QJSON PART
    qjson/src/parser.cpp \
    qjson/src/qobjecthelper.cpp \
    qjson/src/json_scanner.cpp \
    qjson/src/json_parser.cc \
    qjson/src/parserrunnable.cpp \
    qjson/src/serializer.cpp \
    qjson/src/serializerrunnable.cpp
