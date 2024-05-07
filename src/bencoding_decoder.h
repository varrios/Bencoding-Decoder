#ifndef SAMPLE_READ_FILE_H
#define SAMPLE_READ_FILE_H

#include <vector>
#include "string"
#include <fstream>
#include <iostream>
#include <optional>
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

std::vector<char> readFile(const std::string& filePath);

bencodedString decodeString(const std::vector<char>& data, size_t& currentPosition, Dictionary& decodedDataDict);

bencodedInt decodeInteger(const std::vector<char>& data, size_t& currentPosition, Dictionary& decodedDataDict);

bencodedList decodeList(const std::vector<char>& data, size_t& currentPosition, Dictionary& decodedDataDict);

bencodedDict decodeDictionary(const std::vector<char>& data, size_t& currentPosition, Dictionary& decodedDataDict);

bencodedValue decodeBencodedValue(const std::vector<char>& data, size_t& currentPos, Dictionary& decodedDataDict);

void printBencodedValue(const bencodedValue& value, int indent);

void printDictionary(const bencodedDict& dictionary, int indent);

void printRootDictionary(const Dictionary& dictionary);


Dictionary decodeBencodedData(const std::vector<char>& data);


#endif //SAMPLE_READ_FILE_H
