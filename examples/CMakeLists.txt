include_directories(${CMAKE_BINARY_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(facebook-login)
add_subdirectory(foursquare-login)
add_subdirectory(linkedin-login)