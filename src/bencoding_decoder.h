#ifndef SAMPLE_READ_FILE_H
#define SAMPLE_READ_FILE_H

#include <vector>
#include "string"
#include <fstream>
#include <iostream>

struct bencodedValue;

using bencodedList = std::vector<bencodedValue>;
using bencodedDict = std::unordered_map<std::string, bencodedValue>;
using bencodedString = std::string;
using bencodedInt = int;

struct bencodedValue {
    std::variant<bencodedList, bencodedDict, bencodedString, bencodedInt> data;
};


struct Dictionary{
    std::unordered_map<std::string, bencodedValue> keyValueMap;
    bool keyFlag = true;
    std::string lastKeySaved;
    bencodedValue lastValueSaved;

};

std::vector<char> readFile(const std::string& file_path);
std::string decodeBencodedValue(const std::vector<char>& data, size_t& currentPos, Dictionary& encodedDict);
void decodeBencodedData(const std::vector<char>& data);

#endif //SAMPLE_READ_FILE_H
