#ifndef SAMPLE_READ_FILE_H
#define SAMPLE_READ_FILE_H

#include <vector>
#include "string"
#include <fstream>
#include <iostream>


std::vector<char> readFile(const std::string& file_path);
std::string decodeBencodedValue(const std::vector<char>& data, size_t& currentPos);
void decodeBencodedData(const std::vector<char>& data);

#endif //SAMPLE_READ_FILE_H
