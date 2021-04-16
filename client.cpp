#include <iostream>
#include <TCPClient/tcp_client.hpp>
#include <stdexcept>
#include <unistd.h>
int fileContentHandler::port_;
int  fileContentHandler::max_file_size_;
int  fileContentHandler::timeout_sec_;
std::string  fileContentHandler::prefix_;

int main(int argc, char ** argv)
{
    try
    {
        if(argc!=2)
            throw std::logic_error("insira o arquivo alvo");
        fileContentHandler::loadConfigFile("../config_server.txt");
        auto file_data = readFile(argv[1]);
        auto file_data_list = subdivideFile(file_data,fileContentHandler::max_file_size_);

        boost::asio::io_context io_context;
        TCP_Client client(io_context,"127.0.0.1");
        client.start_client();
        std::vector<TCP_Packet> packet_list;

        data2packetlist(file_data,packet_list);
        client.sendNumberPackets(file_data_list.size());
        client.sendNumberPackets(packet_list.size());
        delete file_data.data;

        int total_size = packet_list.size();
        if(packet_list.size()<file_data_list.size())
            total_size = file_data_list.size();
        int cont_packs=1;
        for(auto file:file_data_list){
            packet_list.clear();
            data2packetlist(file,packet_list);

            for(auto pack:packet_list)
            {                  
                client.sendPacket(pack);
                std::cout<<"sended "<<cont_packs<<"/"<<total_size<<"\n";
                cont_packs++;
            }
        }
        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}