VPATH += $${PWD}
INCLUDEPATH += $${PWD}
DEPENDPATH += $${PWD}

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
