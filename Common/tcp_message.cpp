#include "tcp_message.hpp"
#include <iostream>
TCP_Message::TCP_Message()
{
}
/*
bool TCP_Message::decodeHeader()
{
   std::memcpy(header_,raw_,TCP_Message::HEADER_SIZE);
   header_[HEADER_SIZE] ='\0';
   body_length_ = atoi(header_);
   if(body_length_ > TCP_Message::MAX_BODY_SIZE)
   {
      body_length_ = 0;
      return 0;
   }
   return 1;
}
void TCP_Message::encodeHeader()
{
    char header[TCP_Message::HEADER_SIZE+1];
    memcpy(raw_, header, TCP_Message::HEADER_SIZE);
}*/