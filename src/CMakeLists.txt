set(socializer_SRCS
    oauth.cpp
    user.cpp
    facebook/facebook.cpp
    facebook/facebookuser.cpp
    foursquare/foursquare.cpp
    foursquare/foursquareuser.cpp
    linkedin/linkedin.cpp
    linkedin/linkedinuser.cpp
    twitter/twitter.cpp
)

include_directories(facebook)
include_directories(foursquare)
include_directories(linkedin)
include_directories(twitter)

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
