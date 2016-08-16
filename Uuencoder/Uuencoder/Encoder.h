#pragma once

#include <iostream>

class Encoder {
public:
	// default constructor
	Encoder();

	// encode main method, throws exception
	void encode(std::string input, std::string output);

};