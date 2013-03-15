include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(socializer_SRCS
    facebook.cpp
    foursquare.cpp
    foursquareuser.cpp
    linkedin.cpp
    linkedinuser.cpp
    oauth.cpp
    twitter.cpp
)

# final lib
add_library(Socializer STATIC ${socializer_SRCS})

 # link lib with Qt libs
target_link_libraries(Socializer
                      ${QT_QTCORE_LIBRARIES}
                      ${QT_QTNETWORK_LIBRARIES}
                      ${QJSON_LIBRARIES})
