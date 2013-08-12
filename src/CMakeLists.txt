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
target_link_libraries(Socializer)

# qt5_use_modules(Socializer Core Gui Quick Qml Network Xml)
qt5_use_modules(Socializer Core Network)
