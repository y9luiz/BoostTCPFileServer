project(TCPBoostFileTranserServer)

add_subdirectory(TCPConnection)

if(  NOT ${Boost_FOUND})
    find_package(Boost REQUIRED)
endif()
add_library(tcp_server tcp_server.cpp tcp_server.hpp)
target_link_libraries(tcp_server tcp_connection ${Boost_LIBRARIES})
target_include_directories(tcp_server PUBLIC  ${Boost_INCLUDE_DIRS})