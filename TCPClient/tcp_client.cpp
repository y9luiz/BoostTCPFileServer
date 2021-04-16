#include "tcp_client.hpp"
#include <string.h>
#include <iostream>
#include <boost/bind.hpp>

TCP_Client::TCP_Client(boost::asio::io_context& io_context, char * ip)
    : io_context_(io_context), socket_(io_context), timer_(io_context)
{
    boost::asio::ip::tcp::resolver resolver(io_context);
      end_point_ = boost::asio::ip::tcp::endpoint (boost::asio::ip::address::from_string(ip),
      fileContentHandler::port_);
    //end_points = resolver.resolve(query);
}
void TCP_Client::handle_timeout()
{
    
}

bool initialized_timeout = false;
void TCP_Client::startTimeout(){
    
  if (timer_.expires_from_now().seconds() > 0 || !initialized_timeout)
  {
    timer_.expires_from_now(boost::posix_time::seconds(fileContentHandler::timeout_sec_));
    initialized_timeout = true;
    // We managed to cancel the timer. Start new asynchronous wait.
    timer_.cancel_one();
    timer_.async_wait(boost::bind(&TCP_Client::handle_timeout, this));
  }
  else
  {

    socket_.close();
    // Too late, timer has already expired!
  }
  
}
void TCP_Client::start_client()
{
  boost::asio::ip::tcp::resolver::iterator end;
  boost::system::error_code error = boost::asio::error::host_not_found;
  
  socket_.connect(end_point_, error);
  startTimeout();
  if(error)
  {
    std::cout<<error<<"\n";
    std::cout<<"could not to connect "<<"\n";
    exit(0);
  }
  startTimeout();
}

void TCP_Client::read_header()
{
  char  header[TCP_Message::SIZE_INFO::HEADER_SIZE+1];
  socket_.read_some(boost::asio::buffer(header,4));
  header[TCP_Message::SIZE_INFO::HEADER_SIZE] = '\0';
  msg_.getLastPacket().setHeader(header);

}
 void TCP_Client::handle_read_header(const boost::system::error_code&  e /*error*/,
      size_t /*bytes_transferred*/)
{

}
void TCP_Client::sendPacket(std::string msg)
{ 
  startTimeout();

  // pacote com header e body
  // o header é o tamanho do proprio pacote
  char * header = new char [TCP_Message::HEADER_SIZE +1];
  int2MessageHeader(msg.size(),header);
  char * packet = new char [atoi(header)+ TCP_Message::HEADER_SIZE];
  strncpy(packet,header,strlen(header));
  packet[strlen(header)]='\0';
  strncat(packet,msg.c_str(),msg.size());
  delete header;
  // envio header + body
  boost::asio::write(socket_,boost::asio::buffer(packet,msg.size()+TCP_Message::HEADER_SIZE));
}
void TCP_Client::sendPacket(TCP_Packet & packet)
{ 
  startTimeout();
  boost::asio::write(socket_,boost::asio::buffer(packet.getData(),packet.getSize()));
}
void TCP_Client::sendNumberPackets(int n_packets)
{  
  char  p [TCP_Message::HEADER_SIZE +1];
  int2MessageHeader(n_packets,p);
  std::cout<<"enviando numero de pacotes "<<p <<"\n";
  socket_.write_some(boost::asio::buffer(p,4));
}