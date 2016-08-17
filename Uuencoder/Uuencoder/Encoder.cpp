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
    char chunk[3];
    // number of bytes in the line
    int byte_count = 0;
    // 4-byte encoding
    char bytes[4];
    // a single encoded line
    std::string line;

    // size contents to be divisible by 3 with padding 0's if necessary
    if (from.gcount() % 3 == 1){
        from << "0";
    }
    else if (from.gcount() % 3 == 2){
        from << "00";
    }

    // total number of chars in the stream to loop through
    from.seekp(0, std::ios::end);
    int char_count = from.tellp();
    _DPRINT("char_count", char_count);
    from.seekp(0, std::ios::beg);

    for (int i = 0; i < char_count - 1; i += 3){
        _DPRINT("iterator", i);
        from.get(chunk, 4);
        _DPRINT("chunk", chunk);
        // take 3 characters at a time, make 4 6-bit words and add 32 to each
        bytes[0] = (char) ((chunk[0] & 0b11111100) >> 2) + 32;
        bytes[1] = (char) (((chunk[0] & 0b00000011) << 4) | ((chunk[1] & 0b11110000) >> 4)) + 32;
        bytes[2] = (char) (((chunk[1] & 0b00001111) << 2) | ((chunk[2] & 0b11000000) >> 6)) + 32;
        bytes[3] = (char) (chunk[2] & 0b00111111) + 32;
        // add 4 to the total amount of bytes in the row
        byte_count += 3;
        // add bytes the the current line
        line += std::string(bytes);
        _DPRINT("line", line);

        // start on new line every 45 bytes
        if (byte_count % 45 == 0){
            // lines are max of 45 bytes long, which is M uuencoded
            to << "M" << line << "\n";
            // reset line
            line = "";
        }
    }

    if (byte_count % 45 != 0){
        char size = (char) (byte_count % 45) + 32;
        _DPRINT("size", size);

        to << std::string(size + line + "\n");
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
    encoded << "begin 644 " << input << "\n";
    Encoder::appendFile(output, encoded);

	// encode text and append to file
    Encoder::uuencoder(contents, encoded);
    _DPRINT("encoded", encoded.str())
    Encoder::appendFile(output, encoded);

	// add ending to file
    encoded << "`\nend";
    Encoder::appendFile(output, encoded);
}