//#pragma once
#include <string>
#include <algorithm>
#include <vector>
using std::vector;
using std::string;

bool is_number(const std::string& s);
vector<string> split(const string& str, const string& delim);
std::vector<std::string> data2packetlist(std::string data, int max_size_pack);
std::string readFile(char * filename);
void writeFile(char * filename, char * data);