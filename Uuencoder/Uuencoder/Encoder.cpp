#include "Encoder.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

// Debugger mode
#define DEBUG // comment out to turn off debug printer
#ifdef DEBUG
#define _DPRINT(what, val) std::cout << what << ": " << val << std::endl;
#else
#define _DPRINT(what)
# endif

// Encoder default constructor
// @params: none
// @return: none
Encoder::Encoder() {};

// Encoder read file to string
// @method: reads .txt file of specified name and returns the contents as a string or throws exception if file not found
// @params: input - file name
// @return: contents of file as string
void Encoder::readFile(std::string input, std::iostream &contents) {
	std::ifstream reader;
	try {
		reader.open(input.c_str());

        if (!reader.is_open()){
            std::cerr << "Could not open input file " << input << std::endl;
            exit(1);
        }

        contents << reader.rdbuf();
		reader.close();

	} catch (std::fstream::failure e){
		std::cerr << "Exception manipulating file " << input << std::endl;
        std::exit(1);
	}
}


// Encoder append to file method
// @method: writes binary contents to end of output file, err if exception caught
// @params: output - file to write to, contents - text to append to file
// @return: none
void Encoder::appendFile(std::string output, std::iostream &contents) {
    std::fstream appender;
    try{
        appender.open(output.c_str(), std::ios::out | std::ios::app);

        appender << contents.rdbuf();

        appender.close();
    } catch (std::fstream::failure e){
        std::cerr << "Exception writing to " << output << std::endl;
        std::exit(1);
    }
}

// Encoder overwrite file to clear method
// @method: opens file and clears without writing anything to file, err if no file
// @params: file - name of file to overwrite
// @return: none
void Encoder::clearFile(std::string file) {
    std::fstream clearer;
    try{
        clearer.open(file, std::ios::out | std::ios::trunc);
        clearer.close();
    } catch (std::fstream::failure e){
        std::cerr << "Exception overwriting " << file << std::endl;
        std::exit(1);
    }
}

// Encoder uuencoder logic
// @method: encodes the contents of a file using uuencoding
// @params: from - text to be encoded, to - output stream to store encoded text
// @return: string of encoded text
void Encoder::uuencoder(std::iostream &from, std::iostream &to) {
    // pulls 3 characters from stream at a time (3-bytes)
    std::string chunk;
    // number of bytes in the line
    int byte_count = 0;
    // 4-byte encoding
    std::string bytes ("    ");
    // a single encoded line
    std::string line;

    while(getline(from, chunk)){
        // index of current spot in chunk
        int c = 0;
        int c_len = chunk.length();
        _DPRINT("chunk length", chunk.length());
        while (c_len - c - 45 >= 0){
            _DPRINT("writing line", c / 45);
            to << "M";
            for (int i = c; i < c + 45; i += 3){
                bytes[0] = (char) ((chunk[i] & 0b11111100) >> 2) + 32;
                bytes[1] = (char) (((chunk[i] & 0b00000011) << 4) | ((chunk[i+1] & 0b11110000) >> 4)) + 32;
                bytes[2] = (char) (((chunk[i+1] & 0b00001111) << 2) | ((chunk[i+2] & 0b11000000) >> 6)) + 32;
                bytes[3] = (char) (chunk[i+2] & 0b00111111) + 32;
                
                to << bytes;
            }
            to << "\n";
            c += 45;
        }
        
        if (c_len - c > 0){
            if ((chunk.length() - c) % 3 == 1){
                chunk += "00";
                c_len += 2;
            }else if ((chunk.length() - c) % 3 == 2){
                chunk += "0";
                c_len += 1;
            }
            
            int size = (c_len - c);
            
            to << (char) (size + 32);
            for (int i = c; i < c + size; i += 3){
                _DPRINT("iterator", i);
                bytes[0] = (char) ((chunk[i] & 0b11111100) >> 2) + 32;
                bytes[1] = (char) (((chunk[i] & 0b00000011) << 4) | ((chunk[i+1] & 0b11110000) >> 4)) + 32;
                bytes[2] = (char) (((chunk[i+1] & 0b00001111) << 2) | ((chunk[i+2] & 0b11000000) >> 6)) + 32;
                bytes[3] = (char) (chunk[i+2] & 0b00111111) + 32;
                
                to << bytes;
            }
            
            to << "\n";
        }
    }
}

// Encoder main encode
// TODO: add exceptions if encoding fails
// @method: encodes a txt file using uuencoding, throws exception
// @params: input - file to be encoded, output - file to write encoded text to
// @return: none
void Encoder::encode(std::string input, std::string output) {
	// read in input contents first in case outputting to input file
	std::stringstream contents;
    Encoder::readFile(input, contents);
    _DPRINT("contents", contents.str())

    // clear contents of output since appendFile will not
    Encoder::clearFile(output);

	// add header to output
    std::stringstream encoded;
    encoded << "begin 644 " << output << "\n";
    Encoder::appendFile(output, encoded);

	// encode text and append to file
    Encoder::uuencoder(contents, encoded);
    _DPRINT("encoded", encoded.str())
    Encoder::appendFile(output, encoded);

	// add ending to file
    encoded << "`\nend";
    Encoder::appendFile(output, encoded);
}