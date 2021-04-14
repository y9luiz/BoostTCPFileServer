class TCP_Packet
{
    public:
        enum SIZE_INFO{
            HEADER_SIZE = 4,
            BODY_MAX_SIZE = 1500
        };
        TCP_Packet(){
            data_ = NULL;
            size_ = 0;
            header_[HEADER_SIZE];
        }
        TCP_Packet(char *data)
        {
            header_[HEADER_SIZE] = '\0';
            decodePacket(data);
        }
        ~TCP_Packet()
        {
        }
        inline char * getData(){return data_;};
        inline char * getBody(){return body_;};
        inline char * getHeader(){return header_;};
        void setData(char * data, int data_size){
            //decodePacket(data);
            if(data_ == NULL)
                delete data_;
            data_ = new char [data_size];
            memcpy(data_,data,data_size);
        };
        inline void setHeader(char * header)
        {
            memcpy(header_,header,HEADER_SIZE);
            header_[HEADER_SIZE] = '\0';
        }
        inline bool setBody(char * body, int body_size=BODY_MAX_SIZE)
        {
            if(header_!= NULL)
            {
                memcpy(body_,body,body_size);
                strncat(data_,body,body_size);
                size_ = body_size+HEADER_SIZE;
                body_[body_size] = '\0';

                return true;
            }
            return false;
        }
        // decode a input data, if the data could be
        // decoded store it and return true
        // else return false
        bool decodePacket(char *data)
        {
            memcpy(header_,data,HEADER_SIZE);
            header_[HEADER_SIZE+1] = '\0';
            int body_size = atoi(header_);
            if( body_size > BODY_MAX_SIZE +1 )
            {
                return false;                
            }
            setData(data,body_size+HEADER_SIZE);
            size_ = atoi(header_) + HEADER_SIZE;
            return true;
        }
        inline int getBodySize(){return size_-HEADER_SIZE;};
        inline int getSize(){return size_;};
    private:
        char * data_;
        char header_ [HEADER_SIZE+1];
        char body_ [BODY_MAX_SIZE];
        // total size: body + header
        int size_;
};