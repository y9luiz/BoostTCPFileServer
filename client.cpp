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
        writeFile("oi",file_data.data,file_data.size);
        boost::asio::io_context io_context;
        TCP_Client client(io_context,"127.0.0.1");
        client.start_client();

        std::vector<TCP_Packet> packet_list;
        data2packetlist(file_data,packet_list);

        client.sendNumberPackets(packet_list.size());
        for(auto pack:packet_list)
        {                  
            //std::cout<<"pacote "<<pack.getData()<<"\n";
            client.sendPacket(pack);
            //sleep(1);
        }
        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}