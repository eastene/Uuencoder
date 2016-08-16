// Uuencoder.cpp : Defines the entry point for the console application.
//

// Debugger mode
#define DEBUG // comment out to turn off debug printer
#ifdef DEBUG
#define _DPRINT(what) std::cout << what << std::endl;
#else
#define _DPRINT(what)
# endif


#include <iostream>
#include "Encoder.h"
#include "Decoder.h"

// constants
const int NUM__UNIQUE_ARGS = 2;

// if command line arguments are incorrect, display usage and exit
void printUsageAndExit() {
	std::cout << "Usage: uuencode (-(p|s)(d|e)) <input.txt> (output.txt)" << std::endl;
	std::exit(1);
}

// if user passed optional flags, test flags for correctness based on current defined flags
bool isValidFlag(char flag) {
	// current flags, if more flags are added later on should look into a more effecient method for checking
	if (flag != 's' && flag != 'e' && flag != 'p' && flag != 'd') {
		_DPRINT("false flag");
		return false;
	}

	return true;
}

// check flag argument for contradictions
bool containsContradictions(char* arg) {
	// if more flags are added, should find a more effecient method
	if (strchr(arg, 'e') != NULL && strchr(arg, 'd') != NULL) {
		_DPRINT("contains contradiction");
		return true;
	}
	if (strchr(arg, 's') != NULL && strchr(arg, 'p') != NULL) {
		_DPRINT("contains contradiction");
		return true;
	}
	else {
		return false;
	}
}

// find valid flags, or print usage and exit if too many flags, repeated flags, or contradictory flags
char* setFlagsOrExit(char* arg) {
	if (containsContradictions(arg)) {
		printUsageAndExit();
	}
	else if (strlen(arg) > NUM__UNIQUE_ARGS) {
		printUsageAndExit();
	}

	char* uniqueFlags = "";
	for (int i = 0; i < strlen(arg); i++) {
		if (!isValidFlag(arg[i])) {
			printUsageAndExit();
		}
		else if (strchr(uniqueFlags, arg[i]) != NULL) {
			// if a flag is repeated, exit
			_DPRINT("repeated flag")
			printUsageAndExit();
		}
		else {
			uniqueFlags += arg[i];
		}
	}

	return uniqueFlags;
}

// test a command line argument is a .txt file 
bool isTxtFile(char* arg) {
	// find extension in string (if exists)
	char *sub = strstr(arg, ".txt");
	// if no .txt extension, or extension is not at the end of the filename, return false
	if (!sub || strlen(sub) != 4) {
		// if the argument contains .txt but does not end in .txt, return false
		_DPRINT("invalid file")
		return false;
	}
	else {
		// file is .txt
		return true;
	}
}

int main(int argc, char** argv)
{
	// input and output file names
	std::string input, output;
	// begin with flags set to encode and single thread
	char* flags = "es";
    // encoder object
    Encoder encoder;
	
	// check correctness of arguments
	if (argc == 2) {
		if (isTxtFile(argv[1])) {
			// if no flags, and only 1 text file, overwrite input with encoded message
			input = output = argv[1];
		}
		else {
			// not a txt file, exit
			_DPRINT("no file or only flags")
			printUsageAndExit();
		}
	}
	else if (argc == 3) {
		if (isTxtFile(argv[1]) && isTxtFile(argv[2])) {
			// if first argument is txt file, then the second must be as well or exit
			input = argv[1];
			output = argv[2];
		}
		else if (argv[1][0] == '-' && isTxtFile(argv[2])){
			// if first argument is not txt, but 2nd is, set valid flags
			flags = setFlagsOrExit(argv[1]);
			input = argv[2];
		}
		else {
			// if no files, or flags are out of order, exit
			_DPRINT("no files or invalid flags")
			printUsageAndExit();
		}
	}
	else if (argc == 4) {
		// with full args, first must be flags and 2nd and 3rd must be txt files
		if (argv[1][0] == '-' && isTxtFile(argv[2]) && isTxtFile(argv[3])){
			flags = setFlagsOrExit(argv[1]);
			input = argv[2];
			output = argv[3];
		}
		else {
			_DPRINT("arguments in wrong order, invalid flags, or no txt files")
			printUsageAndExit();
		}
	}
	else {
		// must be used with at least 1 file argument and no more than 1 flag argument and 2 file arguments
		_DPRINT("too many or too few arguments")
		printUsageAndExit();
	}

    if (strchr(flags, e)){
        encoder.encode(input, output);
    }

    return 0;
}

