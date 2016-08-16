#pragma once

#include <iostream>

class Encoder {
public:
	// default constructor
	Encoder();

	// read input file to string and return it
	std::string readFile(std::string input);
	// encode main method, throws exception
	void encode(std::string input, std::string output);

};