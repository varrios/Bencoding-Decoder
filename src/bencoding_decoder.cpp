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

bencodedString decodeString(const std::vector<char>& data, size_t& currentPosition, Dictionary& decodedDataDict){

    size_t stringLength = 0;
    while(data[currentPosition] != ':' && currentPosition < data.size()) {
        stringLength = stringLength * 10 + (data[currentPosition] - '0');
        currentPosition++;
    }
    currentPosition++;
    bencodedString decodedString;
    for(size_t i = 0; i < stringLength; i++) {
        decodedString.push_back(data[currentPosition]);
        currentPosition++;
    }
    //std::cout << "Decoded string: " << decodedString << "\n";
    return decodedString;
}

bencodedInt decodeInteger(const std::vector<char>& data, size_t& currentPosition, Dictionary& decodedDataDict) {
    currentPosition++;
    long long integer = 0;
    bool isNegative = false;

    if(data[currentPosition] == '-') {
        isNegative = true;
        currentPosition++;
    }

    while(data[currentPosition] != 'e' && currentPosition < data.size()) {
        integer = integer * 10 + (data[currentPosition] - '0');
        currentPosition++;
    }
    //std::cout << "Decoded integer: " << (isNegative ? -integer : integer) << "\n";
    currentPosition++;
    return isNegative ? bencodedInt{-integer} : bencodedInt{integer};
}

bencodedDict decodeDictionary(const std::vector<char>& data, size_t& currentPosition, Dictionary& decodedDataDict) {
    //std::cout << "Dictionary" << std::endl;
    bencodedDict decodedDictionary;
    currentPosition++;
    while(data[currentPosition] != 'e' && currentPosition < data.size()) {
        bencodedString benString = decodeString(data, currentPosition, decodedDataDict);
        bencodedValue benValue = decodeBencodedValue(data, currentPosition, decodedDataDict);
        decodedDictionary[benString] = benValue;
    }
    currentPosition++;
    return decodedDictionary;
}


bencodedValue decodeBencodedValue(const std::vector<char>& data, size_t& currentPos, Dictionary& decodedDataDict) {
    char c = data[currentPos];
    if(c >= '0' && c <= '9') {
        bencodedString benString = decodeString(data, currentPos, decodedDataDict);
        return bencodedValue{benString};
    }
    else if(c == 'd') {
        bencodedDict benDict = decodeDictionary(data, currentPos, decodedDataDict);
        return bencodedValue{benDict};
    }
    else if(c == 'l') {
        bencodedList benList = decodeList(data, currentPos, decodedDataDict);
        return bencodedValue{benList};
    }
    else if(c == 'i') {
        bencodedInt benInt = decodeInteger(data, currentPos, decodedDataDict);
        return bencodedValue{benInt};
    }
    else {
        std::cerr << "Invalid character found in the data at position: " << currentPos << "\n";
        return {};
    }
}


void printBencodedValue(const bencodedValue& value, int indent) {
    std::visit([&indent](const auto& arg) {
        using T = std::decay_t<decltype(arg)>;
        for(int i = 0; i < indent; i++) {
            std::cout << "  ";
        }
        if constexpr (std::is_same_v<T, bencodedList>)
        {
            std::cout << "\n";
            for(int i = 0; i < indent; i++)
                std::cout << "  ";
            std::cout << "[\n";
            int newIndent = indent + 1;
            for (const auto& v : arg) {
                printBencodedValue(v, newIndent);
            }
            for(int i = 0; i < indent; i++)
                std::cout << "  ";
            std::cout << "]\n";
        }
        else if constexpr (std::is_same_v<T, bencodedDict>)
        {
            std::cout << "\n";
            for(int i = 0; i < indent; i++)
                std::cout << "  ";
            std::cout << "{\n";
            int newIndent = indent + 1;
            printDictionary(arg, newIndent);
            for(int i = 0; i < indent; i++)
                std::cout << "  ";
            std::cout << "}\n";
        }
        else if constexpr (std::is_same_v<T, bencodedString>)
        {
            std::cout << arg << "\n";
        }
        else if constexpr (std::is_same_v<T, bencodedInt>)
        {
            std::cout << arg << "\n";
        }
    }, value.data);
}

void printDictionary(const bencodedDict& dictionary, int indent) {
    for (const auto& [key, value] : dictionary) {
        for(int i = 0; i < indent; i++) {
            std::cout << "  ";
        }
        std::cout << key << ": ";
        printBencodedValue(value, indent);
    }
}

void printRootDictionary(const Dictionary& dictionary) {
    int depth = 1;
    std::cout << "{\n";
    const bencodedValue& value = dictionary.data;
    if (const auto* dictPtr = std::get_if<bencodedDict>(&value.data)) {
        printDictionary(*dictPtr, depth);
    } else {
        std::cerr << "Error: Data is not a dictionary.\n";
    }
    std::cout << "}\n";
}


Dictionary decodeBencodedData(const std::vector<char>& data) {
    std::cout << "Decoding bencoded data...\n";
    size_t currentPos = 0;
    Dictionary decodedDataDict;
    while(currentPos < data.size()) {
        bencodedValue benValue = decodeBencodedValue(data, currentPos, decodedDataDict);
        decodedDataDict.data = benValue;
    }
    return decodedDataDict;

}

bencodedList decodeList(const std::vector<char> &data, size_t &currentPosition, Dictionary &decodedDataDict) {
    //std::cout << "List: " << std::endl;
    bencodedList benList;
    currentPosition++;
    while(data[currentPosition] != 'e' && currentPosition < data.size()) {
        benList.push_back(decodeBencodedValue(data, currentPosition, decodedDataDict));
    }
    currentPosition++;
    return benList;
}

