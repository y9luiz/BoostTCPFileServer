#pragma once
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "TCPConnection/tcp_connection.hpp"
class TCP_Server
{
public:
  TCP_Server(boost::asio::io_context& io_context);
private:
  void start_accept();
  void handle_accept(TCP_Connection::pointer new_connection,
      const boost::system::error_code& error);

  boost::asio::io_context& io_context_;
  tcp::acceptor acceptor_;
};
