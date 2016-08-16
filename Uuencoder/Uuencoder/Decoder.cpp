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
bool Decoder::verifyHeader(std::string input, std::stringstream header) {
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
std::stringstream Decoder::uudecoder(std::stringstream contents) {
    return contents;
}

// Decoder main decode method
// TODO: add exceptions
// @method: decodes uuencoded text files, throws exceptions
// @params: input - name of txt file to decode, output - name of txt file to save decoded text
// @return: none
void Decoder::decode(std::string input, std::string output) {
    // read in contents of encoded message
    std::stringstream contents;
    contents = Encoder::readFile(input);

    // verify header and if incorrect, throw exception
    if (Decoder::verifyHeader(input, contents)){
        // clear output file's contents for overwriting
        Encoder::clearFile(output);

        // decode and append to output file
        std::stringstream decoded;
        decoded = Decoder::uudecoder(contents);
        Encoder::appendFile(output, decoded);
    }else{

    }
}
