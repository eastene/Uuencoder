#pragma once

#include <iostream>
#include <sstream>

class Encoder {
private:
    // uuencoder logic
    void uuencoder(std::iostream &from, std::iostream &to);

protected:
    // methods inhereted by decoder intended for its use as well

    // read input file to string and return it or err if exception raised
    void readFile(std::string input, std::iostream &contents);
    // write to file without overwriting
    void appendFile(std::string output, std::iostream &contents);
    // clear output file for writing
    void clearFile(std::string file);

public:
	// default constructor
	Encoder();

	// encode main method, throws exception
	void encode(std::string input, std::string output);
};