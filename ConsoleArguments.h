#pragma once

/** @brief indicates if there is atleast 1 required argument for the program
 * set to 1 if there are ANY required arguments
 * set to 0 if all arguments are optional
 */
#define HAS_REQUIRED_ARGUMENTS 0

#define MAJORVERSION 1
#define MINORVERSION 0
#define BUGFIX		 0

// change every version


// returned console arguments and default settings
struct ConsoleArgumentSettings{
	// CUSTOM USER VALUES START HERE
	// T name = val; // assign default for optional values

	
	// CUSTOM USER VALUES END HERE
};
















// don't touch anything after this

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

struct Version{
	const unsigned int Major;
	const unsigned int Minor;
	const unsigned int Bugfix;
};

typedef unsigned long(*handle_arg_template)(struct ConsoleArgumentSettings* _CAS, unsigned long argc, const char** argv, unsigned long index, unsigned long index_offset, enum RETURNVAL* ret);

struct nlist {
	const char* fulltext;		 			// fulltext
	char associatedchar; 					// single character equivelent of fulltext
	handle_arg_template handle_function;  	// associated function to execute on argument passing
	const char* briefhelp_description; 		// used for normal ./app -h help description
	const char* extendedhelp_description; 	// used for more detailed argument explanations with ./app -x -h
};

enum RETURNVAL HandleConsoleArgs	(const int   argc, const char **argv);

unsigned long HelpFullString		(const char* text, const char** argv, unsigned long index, const unsigned long argc, enum RETURNVAL* ret_val);
unsigned long HelpSingleChar		(const char* text, const char** argv, unsigned long index, const unsigned long argc, enum RETURNVAL* ret_val, unsigned long index_offset);
unsigned long HelpSingleCharParse	(const char  text, const char** argv, unsigned long index, const unsigned long argc, enum RETURNVAL* ret_val, unsigned long index_offset, const char* fullargname);

void printTooFewArguments			(const char* programname, char argument, const char* fullargname);
void printHelp						(const char* programname, char extended_help);
void printVersion					(const struct Version* V);

struct ConsoleArgumentSettings* getConsoleArgumentSettings		();
struct Version* getSoftwareVersion();
