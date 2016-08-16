#pragma once

#include <iostream>

class Encoder {
public:
	// default constructor
	Encoder();

	// read input file to string and return it or err if exception raised
	std::string readFile(std::string input);
	// write to file without overwriting
	void appendFile(std::string output, std::string contents);
	// clear output file for writing
	void clearFile(std::string file);
	// uuencoder logic
	std::string uuencoder(std::string contents);
	// encode main method, throws exception
	void encode(std::string input, std::string output);

};