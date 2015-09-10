set(socializer_SRCS
    facebook.cpp
    facebookuser.cpp
    foursquare.cpp
    foursquareuser.cpp
    linkedin.cpp
    linkedinuser.cpp
    oauth.cpp
    twitter.cpp
)

# final lib
if (ENABLE_DYNAMIC)
    add_library(Socializer SHARED ${socializer_SRCS})
else ()
    add_library(Socializer STATIC ${socializer_SRCS})
endif(ENABLE_DYNAMIC)

set_target_properties(Socializer PROPERTIES
                        SOVERSION ${SOCIALIZER_ABI_VERSION}
                        VERSION ${SOCIALIZER_VERSION})

qt5_use_modules(Socializer Core Network)
