set(socializer_SRCS
    facebook.cpp
    foursquare.cpp
    oauth.cpp
    twitter.cpp
)

set(socializer_MOC_HDRS
    oauth.h
)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

qt4_wrap_cpp(socializer_MOC_SRCS ${socializer_MOC_HDRS})

# final lib
add_library(Socializer STATIC ${socializer_SRCS} ${socializer_MOC_SRCS})

 # link lib with Qt libs
target_link_libraries(Socializer QtCore ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})