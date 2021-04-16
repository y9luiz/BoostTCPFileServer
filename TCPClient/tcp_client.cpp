#include "tcp_client.hpp"
#include <string.h>
#include <iostream>
TCP_Client::TCP_Client(boost::asio::io_context& io_context, char * ip, int port_num)
    : io_context_(io_context), socket_(io_context)
{
    tcp::resolver resolver(io_context);
      end_point_ = boost::asio::ip::tcp::endpoint (boost::asio::ip::address::from_string(ip),
      port_num);
    //end_points = resolver.resolve(query);
}

void TCP_Client::start_client()
{
  tcp::resolver::iterator end;
  boost::system::error_code error = boost::asio::error::host_not_found;
  
  socket_.connect(end_point_, error);
  if(error)
  {
    std::cout<<error<<"\n";
    std::cout<<"could not to connect "<<"\n";
    exit(0);
  }
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
  boost::asio::write(socket_,boost::asio::buffer(packet.getData(),packet.getSize()));
}
void TCP_Client::sendNumberPackets(int n_packets)
{  
  char  p [TCP_Message::HEADER_SIZE +1];
  int2MessageHeader(n_packets,p);
  std::cout<<"enviando numero de pacotes "<<p <<"\n";
  socket_.write_some(boost::asio::buffer(p,4));
}