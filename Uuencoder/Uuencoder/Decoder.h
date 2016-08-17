#pragma once

#include "Encoder.h"
#include <iostream>
#include <sstream>

class Decoder : public Encoder{
public:
	// default constructor
	Decoder();

	// header verifier
	bool verifyHeader(std::string input, std::iostream &header);
	// decoder logic
	void uudecoder(std::iostream &from, std::iostream &to);
	// main decode method, throws exceptions
	void decode(std::string input, std::string output);
};