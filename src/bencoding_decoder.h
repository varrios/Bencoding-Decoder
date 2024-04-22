#ifndef SAMPLE_READ_FILE_H
#define SAMPLE_READ_FILE_H

#include <vector>
#include "string"
#include <fstream>
#include <iostream>
#include <variant>
#include <unordered_map>

struct bencodedValue;

using bencodedList = std::vector<bencodedValue>;
using bencodedString = std::string;
using bencodedInt = long long;
using bencodedDict = std::unordered_map<bencodedString, bencodedValue>;


struct bencodedValue {
    std::variant<bencodedList, bencodedDict, bencodedString, bencodedInt> data;
};


struct Dictionary{
   bencodedValue data;
};

std::vector<char> readFile(const std::string& file_path);
bencodedValue decodeBencodedValue(const std::vector<char>& data, size_t& currentPos, Dictionary& decodedDataDict);
void decodeBencodedData(const std::vector<char>& data);

void printDictionary(const bencodedDict& dictionary, int indent);
void printBencodedValue(const bencodedValue& value);

#endif //SAMPLE_READ_FILE_H
