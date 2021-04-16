#include "tcp_connection.hpp"
#include "../../utils.hpp"
#include <iostream>
#include <unistd.h>
#include <boost/filesystem.hpp>
TCP_Connection::pointer TCP_Connection::create(boost::asio::io_context& io_context)
{
    return pointer(new TCP_Connection(io_context));
}

 tcp::socket& TCP_Connection::socket()
{
    return socket_;
}

void TCP_Connection::start(){
    raw_data = new char [TCP_Message::HEADER_SIZE];
    start_time_str_ = make_daytime_string();
    dst_folder_ = "./cnx_";dst_folder_+=start_time_str_;
    boost::filesystem::create_directory(dst_folder_);
    dst_folder_+="/";

    boost::asio::async_read(socket_,
        boost::asio::buffer(raw_data,4),
        boost::bind(
          &TCP_Connection::handle_read_n_packets, shared_from_this(),
          boost::asio::placeholders::error));
}

TCP_Connection::TCP_Connection(boost::asio::io_context & io_context) : socket_(io_context)
{
}

void TCP_Connection::handle_write(const boost::system::error_code&  e /*error*/,
      size_t /*bytes_transferred*/)
{
   /* if(!e && message_.getLastPacket().getSize()>0){
        boost::asio::async_write(socket_, boost::asio::buffer(message_.getBody()),
            boost::bind(&TCP_Connection::handle_write, this->shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
        
        message_.clear();
    }
    else{
        socket_.close();
    }*/
}

void TCP_Connection::handle_read_n_packets(const boost::system::error_code&  e /*error*/)
{
    if(!e)
    {
        
        numb_packets = atoi(raw_data);        
        std::cout<<"total de pacotes "<<numb_packets<<"\n";
        delete raw_data;
        raw_data = new char[TCP_Message::HEADER_SIZE+1];
        raw_data[TCP_Message::HEADER_SIZE] = '\0';
        // le o header do pacote atual
        boost::asio::async_read(socket_,
                boost::asio::buffer(raw_data,TCP_Message::HEADER_SIZE),
                boost::bind(
                &TCP_Connection::handle_read_header, shared_from_this(),
                boost::asio::placeholders::error));
    }
}

void TCP_Connection::handle_read_header(const boost::system::error_code&  e /*error*/)
{    

    this->message_.getPacketList().push_back(TCP_Packet());
    int body_len = atoi(raw_data);
    bool decoded = (message_.getLastPacket().decodePacket(raw_data) && body_len>0);
    if(!e && decoded ){
        //delete raw_data;
        raw_data = new char[body_len+1];
        raw_data[body_len] = '\0';
        // le o body do pacote atual
        boost::asio::async_read(socket_,
            boost::asio::buffer(raw_data,body_len),
            boost::bind(
            &TCP_Connection::handle_read_body, shared_from_this(),
            boost::asio::placeholders::error));
        
    }else{
        if(!decoded)
        {
            message_.getPacketList().pop_back();
        }

        std::vector<FileData> file_data_list;
        std::cout<<"total de pacotes na fila "<<message_.getPacketList().size()<<"\n";
        std::string filename = dst_folder_; filename+= fileContentHandler::prefix_; filename += start_time_str_;
        std::cout<<"file "<<filename<<"\n";
        for(auto packet: message_.getPacketList())
        {
            writeFile((char*)filename.c_str(),packet.getData()+4,packet.getSize()-4);
        }

        socket_.close();
    }
}
void TCP_Connection::handle_read_body(const boost::system::error_code&  e)
{

    bool body_setted =  message_.getLastPacket().setBody(raw_data);
    if(!e && body_setted){
        //delete raw_data;
        // le o header do proximo pacote
        //std::cout<<"readed body "<<raw_data<<"\n";

        raw_data = new char [TCP_Message::HEADER_SIZE];
        boost::asio::async_read(socket_,
            boost::asio::buffer(raw_data,TCP_Message::HEADER_SIZE),
            boost::bind(
            &TCP_Connection::handle_read_header, shared_from_this(),
            boost::asio::placeholders::error));
    }
    else{

        //std::cout<<message_.getFullMessage()<<"\n";

        socket_.close();
    }
    
}
