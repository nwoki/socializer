set(FacebookLogin_SRCS
    main.cpp
)

# final exe
add_executable(facebook-login-example ${FacebookLogin_SRCS})

# qt5_use_modules(facebook-login-example Core Gui Widgets Declarative)

target_link_libraries(facebook-login-example Socializer)
