#pragma once

// change every version
struct Version{
	const unsigned int Major  = 0;
	const unsigned int Minor  = 0;
	const unsigned int Bugfix = 0;
};

// returned console arguments and default settings
struct ConsoleArgumentSettings{
	// CUSTOM USER VALUES START HERE
	// T name = val; // assign default for optional values
	// CUSTOM USER VALUES END HERE
};

















typedef unsigned long(*handle_arg_template)(struct ConsoleArgumentSettings* _CAS, unsigned long argc, const char** argv, unsigned long index, unsigned long index_offset, char* ret);

struct nlist {
	const char* fulltext;		 			// fulltext
	char associatedchar; 					// single character equivelent of fulltext
	handle_arg_template handle_function;  	// associated function to execute on argument passing
	const char* briefhelp_description; 		// used for normal ./app -h help description
	const char* extendedhelp_description; 	// used for more detailed argument explanations with ./app -x -h
};

int HandleConsoleArgs		(const int   argc, const char **argv);

unsigned long HelpFullString		(const char* text, const char** argv, unsigned long index, const unsigned long argc, char* ret_val);
unsigned long HelpSingleChar		(const char* text, const char** argv, unsigned long index, const unsigned long argc, char* ret_val, unsigned long index_offset = 0);
unsigned long HelpSingleCharParse	(const char  text, const char** argv, unsigned long index, const unsigned long argc, char* ret_val, unsigned long index_offset, const char* fullargname = "");

void printTooFewArguments	(const char* programname, char argument, const char* fullargname = "");
void printHelp				(const char* programname, char extended_help = '\0');
void printVersion			(const struct Version* V);

void getConsoleArgumentSettings(struct ConsoleArgumentSettings* CAS);
void getSoftwareVersion(struct Version* _Version);