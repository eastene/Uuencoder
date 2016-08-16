#pragma once

#include <iostream>
#include <sstream>

class Encoder {
private:
    // uuencoder logic
    std::stringstream uuencoder(std::stringstream contents);

protected:
    // methods inhereted by decoder intended for its use as well

    // read input file to string and return it or err if exception raised
    std::stringstream readFile(std::string input);
    // write to file without overwriting
    void appendFile(std::string output, std::stringstream contents);
    // clear output file for writing
    void clearFile(std::string file);

public:
	// default constructor
	Encoder();

	// encode main method, throws exception
	void encode(std::string input, std::string output);
};