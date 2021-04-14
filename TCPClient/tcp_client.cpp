#include "tcp_client.hpp"
#include <string.h>
#include <iostream>
TCP_Client::TCP_Client(boost::asio::io_context& io_context, char * ip)
    : io_context_(io_context), socket_(io_context)
{
    tcp::resolver resolver(io_context);
    tcp::resolver::query query(ip,"daytime");

    end_points = resolver.resolve(query);
}

void TCP_Client::start_client()
{
  tcp::resolver::iterator end;
  boost::system::error_code error = boost::asio::error::host_not_found;
  while (error && end_points != end)
  {
    socket_.close();
    socket_.connect(*end_points++, error);
  }
}

void TCP_Client::read_header()
{
  char  header[TCP_Message::SIZE_INFO::HEADER_SIZE+1];
  socket_.read_some(boost::asio::buffer(header,4));
  header[TCP_Message::SIZE_INFO::HEADER_SIZE] = '\0';
  msg_.getLastPacket().setHeader(header);
  std::cout<<"header: "<<is_number( header)<<"\n";

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
  TCP_Message::int2MessageHeader(msg.size(),header);
  char * packet = new char [atoi(header)+ TCP_Message::HEADER_SIZE];
  strncpy(packet,header,strlen(header));
  packet[strlen(header)]='\0';
  strncat(packet,msg.c_str(),msg.size());
  delete header;
  // envio header + body
  std::cout<<"pacote "<<packet<<"\n";
  boost::asio::write(socket_,boost::asio::buffer(packet,msg.size()+TCP_Message::HEADER_SIZE));
}
void TCP_Client::sendNumberPackets(int n_packets)
{  
    std::cout<<"entrou\n";

  char  p [TCP_Message::HEADER_SIZE +1];
  TCP_Message::int2MessageHeader(n_packets,p);
  socket_.write_some(boost::asio::buffer(p,4));
  //delete p ;
}