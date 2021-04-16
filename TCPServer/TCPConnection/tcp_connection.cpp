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
          &TCP_Connection::handle_read_n_files, shared_from_this(),
          boost::asio::placeholders::error));
}

TCP_Connection::TCP_Connection(boost::asio::io_context & io_context) : socket_(io_context)
{
}

void TCP_Connection::handle_write(const boost::system::error_code&  e /*error*/,
      size_t /*bytes_transferred*/)
{
}
void TCP_Connection::handle_read_n_files(const boost::system::error_code&  e /*error*/)
{
    if(!e)
    {
        raw_data[TCP_Packet::HEADER_SIZE]='\0';
        numb_files_  = atoi(raw_data);    
        std::cout<<"quantidade de arquivos "<<numb_files_<<"\n";
    
        delete raw_data;
        raw_data = new char[TCP_Message::HEADER_SIZE+1];
        raw_data[TCP_Message::HEADER_SIZE] = '\0';
        // le o header do pacote atual
        boost::asio::async_read(socket_,
                boost::asio::buffer(raw_data,TCP_Message::HEADER_SIZE),
                boost::bind(
                &TCP_Connection::handle_read_n_packets, shared_from_this(),
                boost::asio::placeholders::error));
    }
}

void TCP_Connection::handle_read_n_packets(const boost::system::error_code&  e /*error*/)
{
    if(!e)
    {
        
        numb_packets_ = atoi(raw_data);        
        std::cout<<"total de pacotes "<<numb_packets_<<"\n";
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
        
        auto packet_list = message_.getPacketList();
        int pos = 0;
        int numb_packs = (numb_packets_)/(numb_files_)+1;
        int remainder = (numb_packets_)%(numb_files_);
        if(remainder)
            numb_files_++;
        for(int i =0;i<numb_files_;i++){
            
            std::string filename = dst_folder_; filename+= fileContentHandler::prefix_; filename += start_time_str_;
            if(numb_files_>1){
                filename+="_";
                filename+=std::to_string(i);
            }
            if(i==numb_files_-1 && i>0)
                numb_packs = remainder;
            for(int j=pos;j<pos+numb_packs;j++)
                 writeFile((char*)filename.c_str(),packet_list[j].getData()+4,packet_list[j].getSize()-4);
            pos +=numb_packs;
        }   
        std::cout<<"files writed at "<<dst_folder_<<"\n";
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
