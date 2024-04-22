#include <iostream>
#include "bencoding_decoder.h"
#include "vector"

int main() {
    std::string filePath = R"(sample_data/corrupt.torrent)";
    std::vector<char> torrentContent = readFile(filePath);
    decodeBencodedData(torrentContent);
}
