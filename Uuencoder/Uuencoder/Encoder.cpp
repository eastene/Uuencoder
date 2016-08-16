#include "Encoder.h"
#include <fstream>
#include <iostream>

// Encoder default constructor
// @params: none
// @return: none
Encoder::Encoder() {};

// Encoder read file to string
// @method: reads .txt file of specified name and returns the contents as a string or throws exception if file not found
// @params: input - file name
// @return: contents of file as string
std::string Encoder::readFile(std::string input) {
	std::string contents;
	std::string temp;
	std::fstream reader;
	try {
		reader.open(input.c_str(), std::ios::in);

		while (!reader.eof()) {
			std::getline(reader, temp);
			contents += temp;
		}
		reader.close();

	} catch (std::fstream::failure e){
		std::err << "Exception manipulating file " << input << std::endl;
	}

	return contents;
}

// Encoder main encode
// TODO: add exceptions if encoding fails
// @method: encodes a txt file using uuencoding, throws exception
// @params: input - file to be encoded, output - file to write encoded text to
// @return: none
void Encoder::encode(std::string input, std::string output) {
	// read in input contents first in case outputting to input file
	std::string contents = Encoder::readFile(input);

	// add header to output


	// encode text line by line

	// add ending to file
}