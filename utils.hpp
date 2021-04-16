#pragma once
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include "Common/tcp_packet.hpp"
using std::vector;
using std::string;

//#####################################################
//              STRUCT THAT HOLDS ALL FILE DATA
//                  INTO A SINGLE POINTER
//#####################################################

struct FileData{
    char * data;
    int size;
    FileData(){};
    FileData(char * data, int size)
    {
        this->data = data;
        this->size = size;
    };
};


//#####################################################
//             USEFUL FUNCTIONS PROTOTYPES
//#####################################################

bool is_number(const std::string& s);
vector<string> split(const string& str, const string& delim);
std::vector<TCP_Packet> data2packetlist(FileData f_data);
void  data2packetlist(FileData f_data, std::vector<TCP_Packet> & packet_list);
FileData &  readFile(char * filename);
std::vector<FileData> subdivideFile(FileData f_data, int max_size);
void writeFile(char * filename, char * data);
void writeFile(char * filename, char * data,int size);
void writeFile(char * filename, FileData & f_data);
void writeFile(char * filename, std::vector<FileData> & data_file_list);
void  int2MessageHeader(int n_packets, char * header);  
std::string make_daytime_string();

//####################################################################
//             CLASS TO LOAD THE INPUT FILE PARAMETERS
//####################################################################


class fileContentHandler{
    #define NUM_FIELDS  4
    public:
        static void loadConfigFile(const char * filename)
        {
            auto f_data = readFile((char*)filename);
            auto splitString = [](string str,const string& delim) {
                
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
            };
            int i =0;
            auto file_lines = splitString(f_data.data,"\n");
            std::vector<std::string>str_list;
            for(auto line:file_lines)
            {
                for(auto word:splitString(line," "))
                {  
                    str_list.push_back(word);
                }
            }
            auto isValidField = [](string str, std::vector<std::string>  valid_fields)
            {
                int i=0;
                for(auto field:valid_fields)
                {
                    if(field == str)
                        return i;
                    i++;
                }
                return -1;
            };
            std::vector<std::string>  valid_fields = std::vector<std::string>({"PORT","MAX_FILE_SIZE","PREFIX_NAME","TIMEOUT"});
            int *fields_ptr = new int[NUM_FIELDS];
            for(int i=0;i<NUM_FIELDS;i++)
                fields_ptr[i] = i;
            for(auto it=str_list.begin();it!=str_list.end();it++)
            {
                int field_idx = isValidField(*it,valid_fields);
                if(field_idx>=0)
                {
                    ++it;
                    switch (field_idx)
                    {
                    case 0:
                        port_ = atoi(it->c_str());
                        break;
                    case 1:
                        max_file_size_ = atoi(it->c_str());
                        break;
                    case 2:
                        prefix_ = *it;
                        break;
                    case 3:
                        timeout_sec_ = atoi(it->c_str());
                        break;
                    default:
                        std::string msg = "the field";
                        msg += *it;
                        msg+="is not propety configured";
                        throw std::logic_error(msg);
                        break;
                    }
                }
            }
        }
        
        friend FileData readFile(const char * filename);
        static int port_;
        static int max_file_size_;
        static std::string prefix_;
        static int timeout_sec_;
    private:
        fileContentHandler() = delete;
        fileContentHandler(fileContentHandler& ) = delete;

};

