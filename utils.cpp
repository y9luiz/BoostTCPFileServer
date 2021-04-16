#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <string.h>
bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
/*
vector<string> splitString(const string& str, const string& delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}*/
void  int2MessageHeader(int n_packets, char * header)
{
    //char* header = new char[HEADER_SIZE];
    for(int i=TCP_Packet::HEADER_SIZE-1;i>=0;i--)
    {
        header[i] = n_packets%10+'0';
        n_packets/=10;
    }
    header[TCP_Packet::HEADER_SIZE] = '\0';
    //return header;
}
std::vector<TCP_Packet> data2packetlist( FileData f_data)
{
    // adicionamos cada caractere do arquivo a um pacote tcp
    int num_packets = f_data.size/(TCP_Packet::BODY_MAX_SIZE+TCP_Packet::HEADER_SIZE)+1;
    std::vector<TCP_Packet> packet_list;

    int remeinder = f_data.size%(TCP_Packet::BODY_MAX_SIZE+TCP_Packet::HEADER_SIZE);
    int packet_size = TCP_Packet::BODY_MAX_SIZE+TCP_Packet::HEADER_SIZE;
    char data[ TCP_Packet::BODY_MAX_SIZE+TCP_Packet::HEADER_SIZE];
    int2MessageHeader(packet_size,data);
    int i = 0;
    for(int i=0;i< num_packets;i++)
    {   
        if(i==num_packets-1){
            packet_size = remeinder;
            int2MessageHeader(packet_size,data);
        }
        memcpy(data+TCP_Packet::HEADER_SIZE,f_data.data+i*packet_size,(i+1)*packet_size);
        //data[TCP_Packet::HEADER_SIZE+packet_size]='\0';
        //std::cout<<"dado "<<data<<"\n";
        packet_list.push_back(TCP_Packet(data));
        //std::cout<<"body "<<packet_list.back().getBody()<<"\n";
        i++;
    }


    return packet_list;

}
void  data2packetlist(FileData f_data, std::vector<TCP_Packet> &  packet_list)
{
    // adicionamos cada caractere do arquivo a um pacote tcp
    int num_packets = f_data.size/(TCP_Packet::BODY_MAX_SIZE)+1;

    int remeinder = f_data.size%(TCP_Packet::BODY_MAX_SIZE);
    
    int packet_size =TCP_Packet::BODY_MAX_SIZE;
    if(f_data.size<packet_size)
        packet_size = f_data.size;
    
    //int i = 0;
    int buff_size =0;
    char * data = new char[ TCP_Packet::BODY_MAX_SIZE+TCP_Packet::HEADER_SIZE];

    for(int i=0;i< num_packets;i++)
    {   
        
        if(i==num_packets-1)
        {
            packet_size = remeinder;
        }
        int2MessageHeader(packet_size,data);

        memcpy(data+TCP_Packet::HEADER_SIZE,f_data.data+buff_size,packet_size);
        buff_size+=packet_size;
        TCP_Packet p (data);
        //std::cout<<"meu data: "<<data<<"\n";
        //std::cout<<"meu data da fonte: "<<p.getSize()<<"\n";
        packet_list.push_back(p);

        memset(data,0,TCP_Packet::BODY_MAX_SIZE+TCP_Packet::HEADER_SIZE);
    }
    delete data;
    int i =0;
   
}
FileData &  readFile(char * filename)
{   
    std::ifstream f(filename,std::ios::binary);
    static FileData d;

    if(f.is_open())
    {
        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(f), {});
        d.size = buffer.size();
        d.data = new char [buffer.size()];
        int i=0;
        for(auto byte:buffer)
        {
            d.data[i] = byte;
            i++;
        }

        f.close();
        return d;
    }
    throw std::logic_error("nao foi possível abrir arquivo de entrada\n");
}
void writeFile(char * filename, char * data){
    std::ofstream f(filename,std::ios::binary);
    std::string output(data);
    if(f.is_open())
    {
        f.write(output.c_str(),output.size());
        //std::cout<<output.size()<<"\n";
        //f<<data;
        f.close();

    }
    else
        throw std::logic_error("nao foi possível abrir arquivo de saída\n");

}
void writeFile(char * filename, FileData & f_data){
    std::ofstream f(filename, std::fstream::app);
    //std::cout<<"f_data "<<f_data.data<<" "<<f_data.size<<"\n";
    if(f.is_open())
    {
        //std::cout<<"escrevendo "<<f_data.data<<" tam "<<f_data.size<<"\n";
        f.write(f_data.data,f_data.size);
        f.close();

    }
    else
        throw std::logic_error("nao foi possível abrir arquivo de saída\n");

}
void writeFile(char * filename, char *data,int size){
    FileData d = FileData(data,size);
    writeFile(filename,d);
}
void writeFile(char * filename, std::vector<FileData> & data_file_list)
{
    //std::cout<<"olha o arquivo\n";
    for(auto data_file:data_file_list)
    {
       //std::cout<<data_file.data<<"\n";
       writeFile(filename,data_file); 

    }
    
}

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  char buffer[30];
  struct tm * timeinfo = localtime (&now);
  strftime (buffer,30,"%Y%m%d%H%M%S",timeinfo);
  return buffer;
}
std::vector<FileData> subdivideFile(FileData f_data, int max_size)
{
    std::vector<FileData> files_list;
    int buffer_size = 0;
    int remeider = f_data.size%max_size;
    int numb_files = f_data.size/max_size +1;
    char ** data= new char *[numb_files];
    int pos= 0;
    int file_size = max_size;
    std::cout<<f_data.size<<" sizao\n";
    for(int i =0;i<numb_files;i++)
    {
        if(i==numb_files-1)
        {
            // add to list
            file_size = remeider;
        }
        data[i] = new char[file_size];
        memcpy(data[i],f_data.data+pos ,file_size);
        files_list.push_back(FileData(data[i],file_size));
        pos+=file_size;

    }  
    return files_list;
}