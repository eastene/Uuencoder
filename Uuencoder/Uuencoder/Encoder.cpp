#include "Encoder.h"
#include <fstream>
#include <iostream>
#include <sstream>

// Encoder default constructor
// @params: none
// @return: none
Encoder::Encoder() {};

// Encoder read file to string
// @method: reads .txt file of specified name and returns the contents as a string or throws exception if file not found
// @params: input - file name
// @return: contents of file as string
std::stringstream Encoder::readFile(std::stringstream input) {
	std::stringstream contents;
	std::fstream reader;
	try {
		reader.open(input.c_str(), std::ios::in);

        contents << reader.rdbuf();

		reader.close();

	} catch (std::fstream::failure e){
		std::cerr << "Exception manipulating file " << input << std::endl;
        std::exit(1);
	}

	return contents;
}


// Encoder append to file method
// @method: writes binary contents to end of output file, err if exception caught
// @params: output - file to write to, contents - text to append to file
// @return: none
void Encoder::appendFile(std::string output, std::stringstream contents) {
    std::fstream appender;
    try{
        appender.open(output.c_str(), std::ios::out | std::ios::app);

        appender << contents;

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
// @params: contents - text to be encoded
// @return: string of encoded text
std::stringstream Encoder::uuencoder(std::stringstream contents) {
    // probably not the fastest way to do this but will use this conversion for now
    std::string in = contents.rdbuf();
    // number of bytes in the line
    int byte_count = 0;
    // 4-byte encoding
    char bytes[4];
    // a single encoded line
    std::string line;
    // full encoded text
    std::string out;

    // size contents to be divisible by 3 with padding 0's if necessary
    if (in.length() % 3 == 1){
        in += "0";
    }
    else if (in.length() % 3 == 2){
        in += "00";
    }

    for (int i = 0; i < in.length(); i += 3){
        // take 3 characters at a time, make 4 6-bit words and add 32 to each
        bytes[0] = (char) ((in[i] & 0b11111100) >> 2) + 32;
        bytes[1] = (char) (((in[i] & 0b00000011) << 4) | ((in[i + 1] & 0b11110000) >> 4)) + 32;
        bytes[2] = (char) (((in[i + 1] & 0b00001111) << 2) | ((in[i + 2] & 0b11000000) >> 6)) + 32;
        bytes[3] = (char) (in[i + 2] & 0b00111111) + 32;
        // add 4 to the total amount of bytes in the row
        byte_count += 3;
        // add bytes the the current line
        line += std::string(bytes);

        // start on new line every 45 bytes
        if (byte_count % 45 == 0){
            // lines are max of 45 bytes long, which is M uuencoded
            out += "M";
            out += line;
            out += "\n";
            // reset line
            line = "";
        }
    }

    if (byte_count % 45 != 0){
        char size = (char) (byte_count % 45) + 32;
        out += size;
        out += line;
        out += "\n";
    }

    contents << out;
    return contents;
}

// Encoder main encode
// TODO: add exceptions if encoding fails
// @method: encodes a txt file using uuencoding, throws exception
// @params: input - file to be encoded, output - file to write encoded text to
// @return: none
void Encoder::encode(std::string input, std::string output) {
	// read in input contents first in case outputting to input file
	std::stringstream contents = Encoder::readFile(input);

    // clear contents of output since appendFile will not
    Encoder::clearFile(output);

	// add header to output
    std::stringstream header;
    header << "begin 644 " << input << "\n";
    Encoder::appendFile(output, header);

	// encode text and append to file
    std::stringstream encoded = Encoder::uuencoder(contents);
    Encoder::appendFile(output, encoded);

	// add ending to file
    std::stringstream footer;
    footer << "`\nend";
    Encoder::appendFile(output, footer);
}