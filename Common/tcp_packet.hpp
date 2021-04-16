#pragma once
#include <cstring>
#include <iostream>

class TCP_Packet
{
    public:
        enum SIZE_INFO{
            HEADER_SIZE = 4,
            BODY_MAX_SIZE = 1500
        };
        TCP_Packet(){

            size_ = 0;
            header_ = NULL;
            body_ = NULL;
        }
        TCP_Packet(char *data)
        {
            //header_[HEADER_SIZE] = '\0';
            decodePacket(data);
        }
        ~TCP_Packet()
        {
            memset(data_,0,HEADER_SIZE+BODY_MAX_SIZE);
            header_ = NULL;
            body_ = NULL;
        }
        inline char * getData(){return data_;};
        inline char * getBody(){return body_;};
        inline char * getHeader(){return header_;};
        void setData(char * data, int data_size){
            //decodePacket(data);
            //data_ = new char [data_size];
            memcpy(data_,data,data_size);
            header_= data_;
            body_ = data_+HEADER_SIZE;
            size_ = data_size;


        };
        inline void setHeader(char * header)
        {
            memcpy(data_,header,HEADER_SIZE);
            header_= data_;
        }
        inline bool setBody(char * body)
        {
            if(header_!= NULL)
            {
                char  header[HEADER_SIZE+1];
                memcpy(header,data_,HEADER_SIZE);
                header[HEADER_SIZE] = '\0';
                int body_size = atoi(header);
                memcpy(data_+HEADER_SIZE,body,body_size);
                size_ = body_size+HEADER_SIZE;

                body_ = data_+HEADER_SIZE;

                return true;
            }
            return false;
        }
        // decode a input data, if the data could be
        // decoded store it and return true
        // else return false
        bool decodePacket(char *data)
        {
         

            char header[HEADER_SIZE+1];
            header[HEADER_SIZE] = '\0';
            memcpy(header,data,HEADER_SIZE);
            int body_size = atoi(header);
            if( body_size > BODY_MAX_SIZE )
            {
                return false;                
            }
            setData(data,body_size+HEADER_SIZE);
            return true;
        }
        inline int getBodySize(){return size_-HEADER_SIZE;};
        inline int getSize(){return size_;};
    private:
        char  data_[HEADER_SIZE+BODY_MAX_SIZE];
        char * header_=NULL;
        char * body_=NULL;
        // total size: body + header
        int size_;
};