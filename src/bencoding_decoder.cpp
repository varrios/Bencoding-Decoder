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

std::vector<std::string> decodeList(const std::vector<char>& data, size_t& currentPosition, Dictionary& encodedDict) {
    std::cout << "List: " << std::endl;
    std::vector<std::string> decodedList;
    currentPosition++;
    while(data[currentPosition] != 'e' && currentPosition < data.size()) {
        decodedList.push_back(decodeBencodedValue(data, currentPosition, encodedDict));
    }
    currentPosition++;
    return decodedList;
}

std::unordered_map<std::string, std::string> decodeDictionary(const std::vector<char>& data, size_t& currentPosition, Dictionary& encodedDict) {
    std::cout << "Dictionary" << std::endl;
    std::unordered_map<std::string, std::string> decodedDictionary;
    currentPosition++;
    while(data[currentPosition] != 'e' && currentPosition < data.size()) {
        std::string key = decodeString(data, currentPosition);
        std::string value = decodeBencodedValue(data, currentPosition, encodedDict);
        decodedDictionary[key] = value;
    }
    currentPosition++;
    return decodedDictionary;
}


std::string decodeBencodedValue(const std::vector<char>& data, size_t& currentPos, Dictionary& encodedDict) {
    char c = data[currentPos];
    if(c >= '0' && c <= '9') {
        std::string decoded = decodeString(data, currentPos);
        if(encodedDict.keyFlag) {
            encodedDict.lastKeySaved = decoded;
            encodedDict.keyFlag = false;
        }
        else {
            bencodedValue value;
            value.data = bencodedString{decoded};
            encodedDict.keyValueMap[encodedDict.lastKeySaved] = value;
            encodedDict.keyFlag = true;
        }
        return decoded;
    }
    else if(c == 'd') {
        decodeDictionary(data, currentPos, encodedDict);
        return "Dictionary";
    }
    else if(c == 'l') {
        decodeList(data, currentPos, encodedDict);
        return "List";
    }
    else if(c == 'i') {
        int decoded = decodeInteger(data, currentPos);
        bencodedValue value;
        value.data = bencodedInt{decoded};
        encodedDict.keyValueMap[encodedDict.lastKeySaved] = value;
        encodedDict.keyFlag = true;
        return std::to_string(decoded);
    }
    else {
        std::cerr << "Invalid character found in the data at position: " << currentPos << "\n";
        return "INVALID DATA";
    }
}


void decodeBencodedData(const std::vector<char>& data) {
    Dictionary encodedDict;
    std::cout << "Decoding bencoded data...\n";
    size_t currentPos = 0;
    while(currentPos < data.size()) {
        std::string decoded = decodeBencodedValue(data, currentPos, encodedDict);
        std::cout << "Decoded value: " << decoded << "\n";
    }
    std::cout << " ";
}
