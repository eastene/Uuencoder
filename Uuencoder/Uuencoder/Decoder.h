#pragma once

#include <iostream>
class Decoder {
public:
	// default constructor
	Decoder();

	// main decode method, throws exceptions
	void decode(std::string input, std::string output);
};