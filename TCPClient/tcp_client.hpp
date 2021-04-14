#pragma once
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "../Common/tcp_message.hpp"
#include "../utils.hpp"

using boost::asio::ip::tcp;
class TCP_Client
{
  public:
    TCP_Client(boost::asio::io_context& io_context,char * ip);
    void start_client();
    void sendPacket(std::string msg);
    void sendNumberPackets(int n_packets);
  private:
    void read_header();
    boost::asio::io_context& io_context_;
    tcp::resolver::results_type end_points;
    //tcp::resolver resolver_;
    TCP_Message msg_;
    boost::asio::ip::tcp::socket socket_;
    static void handle_read_header(const boost::system::error_code&  e /*error*/,
        size_t /*bytes_transferred*/); 
};
