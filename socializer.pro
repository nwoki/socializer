TEMPLATE = lib
TARGET = Socializer

CONFIG += create_prl \
        static

DEPENDPATH += . \
              src \
              testApp \

INCLUDEPATH += . src include

UI_DIR = qmakeBuild/
MOC_DIR = qmakeBuild/
OBJECTS_DIR = qmakeBuild/
DESTDIR = lib/

# Input
HEADERS += include/Foursquare \
           include/Facebook \
           include/Twitter \
           src/facebook.h \
           src/foursquare.h \
           src/oauth.h \
           src/twitter.h

SOURCES += src/facebook.cpp \
           src/foursquare.cpp \
           src/oauth.cpp \
           src/twitter.cpp
