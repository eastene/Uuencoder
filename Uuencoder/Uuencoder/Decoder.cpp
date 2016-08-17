#include "Decoder.h"
#include <sstream>

// Debugger mode
#define DEBUG // comment out to turn off debug printer
#ifdef DEBUG
#define _DPRINT(what, val) std::cout << what << ": " << val << std::endl;
#else
#define _DPRINT(what)
# endif

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
    std::string words ("begin 644 " + input);

    getline(header, temp);
    if (temp != words){
        return false;
    }
    return true;
}

// Decoder uudecoder logic
// @method: decodes text encoded with uuencoding
// @params: contents - encoded text
// @return: decoded string
void Decoder::uudecoder(std::iostream &from, std::iostream &to) {
    // size of the line
    int size;
    // 3 decoded letters from 4 encoded bytes
    std::string let("   ");
    // current line of encoded text
    std::string line;
    
    // get stream size
    from.seekp(0, std::ios::end);
    int char_count = from.tellp();
    from.seekp(0, std::ios::beg);

    while (getline(from, line)){
        // get and convert size
        size = line[0];

        if (size == '`'){
            break;
        }

        size = size - 32;
        _DPRINT("size", size);
        
        for (int i = 1; i < line.length(); i += 4){
            let[0] = (((line[i] - 32) & 0b00111111) << 2) | (((line[i+1] - 32) & 0b00110000) >> 4);
            let[1] = (((line[i+1] - 32) & 0b00001111) << 4) | (((line[i+2] - 32) & 0b00111100) >> 2);
            let[2] = (((line[i+2] - 32) & 0b00000011) << 6) | (((line[i+3] - 32) & 0b00111111));
            _DPRINT("letters", let);
            
            // remove padding 0s if needed
            if (i > line.length() - 5){
                if (size % 3 == 1){
                    to << let[0];
                }else if (size % 3 == 2){
                    to << let[0] << let[1];
                }else{
                    to << let;
                }
            }
            else{
                to << let;
            }
        }
    }
}

// Decoder main decode method
// TODO: add exceptions
// @method: decodes uuencoded text files, throws exceptions
// @params: input - name of txt file to decode, output - name of txt file to save decoded text
// @return: none
void Decoder::decode(std::string input, std::string output) {
    _DPRINT("input", input);
    _DPRINT("output", output);
    // read in contents of encoded message
    std::stringstream contents;
    Encoder::readFile(input, contents);
    _DPRINT("contents", contents.str());

    // verify header and if incorrect, throw exception
    if (Decoder::verifyHeader(input, contents)){
        // clear output file's contents for overwriting
        Encoder::clearFile(output);

        // decode and append to output file
        std::stringstream decoded;
        Decoder::uudecoder(contents, decoded);
        _DPRINT("decoded", decoded.str());

        Encoder::appendFile(output, decoded);
    }else{

    }
}
