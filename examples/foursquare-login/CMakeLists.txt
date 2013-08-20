set(FoursquareLogin_SRCS
    main.cpp
)

# final exe
add_executable(foursquare-login-example ${FoursquareLogin_SRCS})

# qt5_use_modules(facebook-login-example Core Gui Widgets Declarative)

target_link_libraries(foursquare-login-example Socializer)
