#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include "tcp_packet.hpp"
#include <deque>
class TCP_Message{
    public:
        TCP_Message();
        TCP_Message(std::string  & m);
        ~TCP_Message(){
            packet_deque_.clear();
        };
        bool decodeHeader();
        void encodeHeader();
        /*inline std::string getHeader()
        {
            return this->header_;
        }

        inline void setHeader(std::string header)
        {
            memcpy(header_,header.c_str(),header.size());
            memcpy(raw_,header.c_str(),header.size());


        }
        inline void setBody(std::string body)
        {
            this->body_ = body;
        }
        inline std::string getBody()
        {
            return this->body_;
        }
        inline char *getData()
        {
            return this->raw_;
        }*/
        inline std::string  getRawMessage()
        {
            std::string raw_msg;
            for(auto packet:packet_deque_)
                raw_msg+=packet.getData();
            return raw_msg;
        }
        inline std::string getFullMessage()
        {
            std::string full_msg = "";
            for(auto packet:packet_deque_)
            {
                full_msg += packet.getBody();
            }
            return full_msg;
        }

        enum SIZE_INFO{
            HEADER_SIZE = 4,
            MAX_BODY_SIZE = 1500
        };
        static  void  int2MessageHeader(int n_packets, char * header)
        {
            //char* header = new char[HEADER_SIZE];
            for(int i=HEADER_SIZE-1;i>=0;i--)
            {
                header[i] = n_packets%10+'0';
                n_packets/=10;
            }
            header[HEADER_SIZE] = '\0';
            //return header;
        }
        int n_packets =0;
        int body_length_;
        TCP_Packet& getLastPacket()
        {
            return packet_deque_.back();
        }
        std::deque<TCP_Packet> & getPacketList()
        {
            return packet_deque_;
        }
    private:
        
        std::deque<TCP_Packet> packet_deque_;
        
};