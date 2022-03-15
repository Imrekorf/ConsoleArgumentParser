#pragma once

/** @brief the amount of console arguments that should 
 * 	atleast be passed for the program to run, either 0 or 1, 
 * 	either all arguments are optional or there are required 
 * 	arguments with -h as a minimum */
#define MINIMAL_ARGS_FOR_PROGRAM 1

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


















enum RETURNVAL {
	NORMAL_COND 	= 00,

	// special exit condition
	RET_AFTER_DONE 	= 10,

	// errors

	/** @brief not enough arguments passed, or less than minimum arguments passed to the program */
	TOO_FEW_ARGS 	= 20,
	NO_ARGS_PARSED 	= 30,
	UNKOWN_ARG 		= 40,
};

typedef unsigned long(*handle_arg_template)(struct ConsoleArgumentSettings* _CAS, unsigned long argc, const char** argv, unsigned long index, unsigned long index_offset, RETURNVAL* ret);

struct nlist {
	const char* fulltext;		 			// fulltext
	char associatedchar; 					// single character equivelent of fulltext
	handle_arg_template handle_function;  	// associated function to execute on argument passing
	const char* briefhelp_description; 		// used for normal ./app -h help description
	const char* extendedhelp_description; 	// used for more detailed argument explanations with ./app -x -h
};

RETURNVAL HandleConsoleArgs			(const int   argc, const char **argv);

unsigned long HelpFullString		(const char* text, const char** argv, unsigned long index, const unsigned long argc, RETURNVAL* ret_val);
unsigned long HelpSingleChar		(const char* text, const char** argv, unsigned long index, const unsigned long argc, RETURNVAL* ret_val, unsigned long index_offset = 0);
unsigned long HelpSingleCharParse	(const char  text, const char** argv, unsigned long index, const unsigned long argc, RETURNVAL* ret_val, unsigned long index_offset, const char* fullargname = "");

void printTooFewArguments			(const char* programname, char argument, const char* fullargname = "");
void printHelp						(const char* programname, char extended_help = '\0');
void printVersion					(const struct Version* V);

void getConsoleArgumentSettings		(struct ConsoleArgumentSettings* CAS);
void getSoftwareVersion				(struct Version* _Version);