# Test libraries
set(TestLibraries ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} Socializer)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_AUTOMOC ON)


# FacebookUserTest
set(facebookusertest_SRCS facebookusertest.cpp)
add_executable(FacebookUserTest ${facebookusertest_SRCS})
target_link_libraries(FacebookUserTest Socializer Qt5::Core Qt5::Test)
add_test(NAME FacebookUserTest COMMAND FacebookUserTest)

# LinkedinUserTest
set(linkedinusertest_SRCS linkedinusertest.cpp)
add_executable(LinkedinUserTest ${linkedinusertest_SRCS})
target_link_libraries(LinkedinUserTest Socializer Qt5::Core Qt5::Test)
add_test(NAME LinkedinUserTest COMMAND LinkedinUserTest)
