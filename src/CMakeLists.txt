set(socializer_SRCS
    facebook.cpp
    foursquare.cpp
    twitter.cpp
    oauth.cpp
)

set(socializer_MOC_HDRS

)

qt4_wrap_cpp(socializer_MOC_SRCS ${socializer_MOC_HDRS})

# final lib
add_library(Socializer STATIC ${socializer_SRCS} ${socializer_MOC_SRCS})

 # link lib with Qt libs
target_link_libraries(Socializer QtCore QtNetwork ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})