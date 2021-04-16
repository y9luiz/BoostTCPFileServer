#include <iostream>
#include <Common/tcp_message.hpp>
#include <TCPServer/tcp_server.hpp>
#include <boost/asio.hpp>
int fileContentHandler::port_;
int  fileContentHandler::max_file_size_;
std::string  fileContentHandler::prefix_;
int  fileContentHandler::timeout_sec_;
int main()
{
    try
  { 
    fileContentHandler::loadConfigFile("../config_server.txt");
    std::cout<<"porta "<< fileContentHandler::port_<<"\n";
    std::cout<<"max_file_size_ "<< fileContentHandler::max_file_size_<<"\n";
    std::cout<<"prefix_ "<< fileContentHandler::prefix_<<"\n";
    std::cout<<"timeout_sec_ "<< fileContentHandler::timeout_sec_<<"\n";

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