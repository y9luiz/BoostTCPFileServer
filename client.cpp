#include <iostream>
#include <TCPClient/tcp_client.hpp>
#include <stdexcept>
#include <unistd.h>
int main(int argc, char ** argv)
{
    try
    {
        if(argc!=2)
            throw std::logic_error("insira o arquivo alvo");
        auto file_data = readFile(argv[1]);
        boost::asio::io_context io_context;
        TCP_Client client(io_context,"127.0.0.1");
        client.start_client();
        auto packet_list = data2packetlist(file_data,TCP_Message::MAX_BODY_SIZE);
        client.sendNumberPackets(packet_list.size());
        std::cout<<"quantidade de pacotes "<<packet_list.size()<<"\n";
        for(auto pack:packet_list)
        {                   
            //std::cout<<pack<<"\n";
            client.sendPacket(pack);
        }
        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}