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
    twitter.h

SOURCES += \
    facebook.cpp \
    foursquare.cpp \
    linkedin.cpp \
    oauth.cpp \
    twitter.cpp
