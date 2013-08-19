set(LinkedinLogin_SRCS
    main.cpp
)

# final exe
add_executable(linkedin-login-example ${LinkedinLogin_SRCS})

target_link_libraries(linkedin-login-example Socializer)
