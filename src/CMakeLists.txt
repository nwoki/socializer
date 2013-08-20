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

if (QT5_BUILD)
    qt5_use_modules(Socializer Core Network)
else()
    target_link_libraries(Socializer ${QT_QTCORE_LIBRARIES} ${QT_QTNETWORK_LIBRARIES} qjson)
endif(QT5_BUILD)
