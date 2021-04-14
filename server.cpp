#include <iostream>
#include <Common/tcp_message.hpp>
#include <TCPServer/tcp_server.hpp>
#include <boost/asio.hpp>
int main()
{
    try
  {
    boost::asio::io_context io_context;
    TCP_Server server(io_context);
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
    return 0;
}