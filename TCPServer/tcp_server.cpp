#include "tcp_server.hpp"

TCP_Server::TCP_Server(boost::asio::io_context& io_context)
    : io_context_(io_context),
      acceptor_(io_context, tcp::endpoint(tcp::v4(), 13))
  {
    start_accept();
  }

void TCP_Server::start_accept()
{
    TCP_Connection::pointer new_connection =
    TCP_Connection::create(io_context_);
    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&TCP_Server::handle_accept, this, new_connection,
          boost::asio::placeholders::error));
}
void TCP_Server::handle_accept(TCP_Connection::pointer new_connection,
      const boost::system::error_code& error)
{
    if (!error)
    {
      new_connection->start();
    }

    start_accept();
}