#include <optional>
#include <unordered_map>
#include "bencoding_decoder.h"

std::vector<char> readFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Failed to open the .torrent file at " << filePath << "\n";
        return {};
    }
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::cout << "Succesfully opened file contents. Size of the file: " << buffer.size() << " bytes\n";
    return buffer;
}

std::string decodeString(const std::vector<char>& data, size_t& currentPosition){

    size_t stringLength = 0;
    while(data[currentPosition] != ':' && currentPosition < data.size()) {
        stringLength = stringLength * 10 + (data[currentPosition] - '0');
        currentPosition++;
    }
    currentPosition++;
    std::string decodedString;
    for(size_t i = 0; i < stringLength; i++) {
        decodedString.push_back(data[currentPosition]);
        currentPosition++;
    }
    std::cout << "Decoded string: " << decodedString << "\n";
    return decodedString;
}

int decodeInteger(const std::vector<char>& data, size_t& currentPosition) {
    currentPosition++;
    int integer = 0;
    bool isNegative = false;

    if(data[currentPosition] == '-') {
        isNegative = true;
        currentPosition++;
    }

    while(data[currentPosition] != 'e' && currentPosition < data.size()) {
        integer = integer * 10 + (data[currentPosition] - '0');
        currentPosition++;
    }
    std::cout << "Decoded integer: " << (isNegative ? -integer : integer) << "\n";
    currentPosition++;
    return isNegative ? -integer : integer;
}

std::vector<std::string> decodeList(const std::vector<char>& data, size_t& currentPosition) {
    std::vector<std::string> decodedList;
    currentPosition++;
    while(data[currentPosition] != 'e' && currentPosition < data.size()) {
        decodedList.push_back(decodeBencodedValue(data, currentPosition));
    }
    currentPosition++;
    return decodedList;
}

std::unordered_map<std::string, std::string> decodeDictionary(const std::vector<char>& data, size_t& currentPosition) {
    std::unordered_map<std::string, std::string> decodedDictionary;
    currentPosition++;
    while(data[currentPosition] != 'e' && currentPosition < data.size()) {
        std::string key = decodeString(data, currentPosition);
        std::string value = decodeBencodedValue(data, currentPosition);
        decodedDictionary[key] = value;
    }
    currentPosition++;
    return decodedDictionary;
}


std::string decodeBencodedValue(const std::vector<char>& data, size_t& currentPos) {
    char c = data[currentPos];
    if(c >= '0' && c <= '9') {
        return decodeString(data, currentPos);
    }
    else if(c == 'd') {
        decodeDictionary(data, currentPos);
        return "Dictionary";
    }
    else if(c == 'l') {
        decodeList(data, currentPos);
        return "List";
    }
    else if(c == 'i') {
        return std::to_string(decodeInteger(data, currentPos));
    }
    else {
        std::cerr << "Invalid character found in the data at position: " << currentPos << "\n";
        return "INVALID DATA";
    }
}


void decodeBencodedData(const std::vector<char>& data) {
    std::cout << "Decoding bencoded data...\n";
    size_t currentPos = 0;
    while(currentPos < data.size()) {
        std::string decoded = decodeBencodedValue(data, currentPos);
        std::cout << "Decoded value: " << decoded << "\n";
    }
}
