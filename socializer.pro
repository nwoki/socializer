TEMPLATE = lib
TARGET = Socializer

CONFIG += create_prl \
        static

DEPENDPATH += . \
              src \
              testApp \

INCLUDEPATH += . src include

include(socializer.pri)

UI_DIR = qmakeBuild/
MOC_DIR = qmakeBuild/
OBJECTS_DIR = qmakeBuild/
DESTDIR = lib/

