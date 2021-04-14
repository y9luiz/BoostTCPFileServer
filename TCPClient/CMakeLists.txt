project(TCPBoostFileTranserServer)


if(  NOT ${Boost_FOUND})
    find_package(Boost REQUIRED)
endif()
add_library(tcp_client tcp_client.cpp tcp_client.hpp)
target_link_libraries(tcp_client  ${Boost_LIBRARIES})
target_include_directories(tcp_client PUBLIC  ${Boost_INCLUDE_DIRS})