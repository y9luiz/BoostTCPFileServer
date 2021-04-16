#pragma once
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "../Common/tcp_message.hpp"
#include "../Common/tcp_packet.hpp"
#include "../utils.hpp"

class TCP_Client
{
  public:
    TCP_Client(boost::asio::io_context& io_context,char * ip);
    void start_client();
    void sendPacket(std::string msg);
    void sendPacket(TCP_Packet & packet);
    void sendNumberPackets(int n_packets);
  private:
    boost::asio::ip::tcp::endpoint end_point_;
    void read_header();
    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::resolver::results_type end_points;
    //tcp::resolver resolver_;
    TCP_Message msg_;
    boost::asio::ip::tcp::socket socket_;
    static void handle_read_header(const boost::system::error_code&  e /*error*/,
        size_t /*bytes_transferred*/); 
    boost::asio::deadline_timer timer_;
    void startTimeout();
    void handle_timeout();
};
