project(TCPBoostFileTranserServer)


if( NOT  ${Boost_FOUND})
    find_package(Boost REQUIRED)
endif()
add_library(tcp_connection tcp_connection.cpp)
target_include_directories(tcp_connection PUBLIC  ${Boost_INCLUDE_DIRS})