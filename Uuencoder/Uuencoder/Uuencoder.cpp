// Uuencoder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

int main(int argc, char** argv)
{
	std::string input, output;

	if (argc < 2 || argc > 3) {
		std::cout << "Usage: uuencode <input.txt> (output.txt)" << std::endl;
		std::exit(1);
	}
	
	if (argc == 2) {
		input = output = argv[1];
	}

	if (argc == 3) {
		input = argv[1];
		output = argv[2];
	}

    return 0;
}

