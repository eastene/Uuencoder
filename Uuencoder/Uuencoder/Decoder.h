#pragma once

#include "Encoder.h"
#include <iostream>
#include <sstream>

class Decoder : public Encoder{
public:
	// default constructor
	Decoder();

	// header verifier
	bool verifyHeader(std::string input, std::stringstream header);
	// decoder logic
	std::stringstream uudecoder(std::stringstream contents);
	// main decode method, throws exceptions
	void decode(std::string input, std::string output);
};