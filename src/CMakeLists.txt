set(QT_USE_QTCORE TRUE)
set(QT_DONT_USE_QTGUI TRUE)

set(socializer_SRCS
    facebook.cpp
    foursquare.cpp
    linkedin.cpp
    oauth.cpp
    twitter.cpp
)

set(socializer_MOC_HDRS
    facebook.h
    foursquare.h
    linkedin.h
    oauth.h
    twitter.h
)

qt4_wrap_cpp(socializer_MOC_SRCS ${socializer_MOC_HDRS})

# final lib
add_library(Socializer STATIC ${socializer_SRCS} ${socializer_MOC_SRCS})

 # link lib with Qt libs
target_link_libraries(Socializer
                      ${QT_QTCORE_LIBRARIES}
                      ${QT_QTNETWORK_LIBRARIES}
                      ${CMAKE_THREAD_LIBS_INIT}
                      ${CMAKE_DL_LIBS}
                      qjson)
