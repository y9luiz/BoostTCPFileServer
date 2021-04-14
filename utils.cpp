#include "utils.hpp"
#include <fstream>
#include <iostream>
bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
vector<string> split(const string& str, const string& delim)
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
}
std::vector<std::string> data2packetlist(std::string data, int max_size_pack)
{
    std::vector<std::string> packet_list;
    std::string packet;
    for(int i=0;i<data.size();i++)
    {
        packet+=data[i];
        if(i%max_size_pack==0 && i>0)
        {
            packet_list.push_back(packet);
            packet.clear();
        }

    }
    if(!packet.empty())
    {
            packet_list.push_back(packet);

    }
    return packet_list;

}
std::string readFile(char * filename)
{   
    std::ifstream f(filename,std::ios::binary);
    if(f.is_open())
    {
        std::string data;
        std::string output;

        char buffer[1024];
        while(f.read(buffer,sizeof(buffer)))
        {
            data.assign(buffer);
            output+=data;
        }
        //std::cout<<data<<"\n";
        f.close();
        
        return output;
    }
    throw std::logic_error("nao foi possível abrir arquivo de entrada\n");
}
void writeFile(char * filename, char * data){
    std::ofstream f(filename,std::ios::binary);
    std::string output(data);
    if(f.is_open())
    {
        f.write(output.c_str(),output.size());
        //f<<data;
        f.close();

    }
    else
        throw std::logic_error("nao foi possível abrir arquivo de saída\n");

}