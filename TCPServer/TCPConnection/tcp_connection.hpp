
#pragma once
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "../../Common/tcp_message.hpp"
#include "../../utils.hpp"
using boost::asio::ip::tcp;

class TCP_Connection : public boost::enable_shared_from_this<TCP_Connection>
{
public:
  typedef boost::shared_ptr<TCP_Connection> pointer;

  static pointer create(boost::asio::io_context& io_context);

  tcp::socket& socket();

  void start();

private:
  TCP_Connection(boost::asio::io_context& io_context);

  void handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/);
  void handle_read_n_packets(const boost::system::error_code&  e /*error*/);
  void handle_read_next_packet_header(const boost::system::error_code&  e /*error*/);
  void handle_read_header(const boost::system::error_code& /*error*/);
  void handle_read_body(const boost::system::error_code& /*error*/);
  friend std::vector<std::string> data2packetlist(std::string data, int max_size_pack);
  tcp::socket socket_; 
  char *raw_data;
  TCP_Message message_;
  int numb_packets=0;
  std::string friend make_daytime_string();
  std::string start_time_str_;
  std::string dst_folder_;
};


