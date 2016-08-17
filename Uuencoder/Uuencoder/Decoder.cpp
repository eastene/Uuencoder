#include "Decoder.h"
#include <sstream>

// Decoder default constructor
// @params: none
// @return: none
Decoder::Decoder() {};

// Decoder header verifier
// @method: verifies file header is from uuencoded file
// @params: input - name of the file to verify, contents - file contents including header
// @return: true if file verified, false otherwise
bool Decoder::verifyHeader(std::string input, std::iostream &header) {
    // temporary string to accept next token
    std::string temp;
    // words that must be in order in header
    std::string words[] = {"begin", "644", input};

    for (int i = 0; i < 3; i++){
        header >> temp;
        if (temp != words[i]){
            return false;
        }
    }
    return true;
}

// Decoder uudecoder logic
// @method: decodes text encoded with uuencoding
// @params: contents - encoded text
// @return: decoded string
void Decoder::uudecoder(std::iostream &from, std::iostream &to) {
    // takes 4 characters at a time to decode
    char chunk[4];
    // size of the line
    char size;
    // 3 decoded letters from 4 encoded bytes
    char let[3];

    // get stream size
    from.seekp(0, std::ios::end);
    int char_count = from.tellp();
    from.seekp(0, std::ios::beg);

    int j = 0;
    while (j < char_count){
        // get and convert size
        from.get(size);

        if (size == '`'){
            break;
        }

        j++;
        size = size - 32;

        for (int i = 0; i < (int) size; i += 4){
            from.get(chunk, 5);
            let[0] = (((chunk[0] - 32) & 0b00111111) << 2) | (((chunk[1] - 32) & 0b00110000) >> 4);
            let[1] = (((chunk[1] - 32) & 0b00001111) << 4) | (((chunk[2] - 32) & 0b00111100) >> 2);
            let[2] = (((chunk[2] - 32) & 0b00000011) << 6) | (((chunk[3] - 32) & 0b00111111));
            j += 4;

            to << std::string(let);
        }
    }
}

// Decoder main decode method
// TODO: add exceptions
// @method: decodes uuencoded text files, throws exceptions
// @params: input - name of txt file to decode, output - name of txt file to save decoded text
// @return: none
void Decoder::decode(std::string input, std::string output) {
    // read in contents of encoded message
    std::stringstream contents;
    Encoder::readFile(input, contents);

    // verify header and if incorrect, throw exception
    if (Decoder::verifyHeader(input, contents)){
        // clear output file's contents for overwriting
        Encoder::clearFile(output);

        // decode and append to output file
        std::stringstream decoded;
        Decoder::uudecoder(contents, decoded);

        Encoder::appendFile(output, decoded);
    }else{

    }
}
