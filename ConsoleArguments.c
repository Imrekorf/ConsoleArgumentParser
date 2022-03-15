#include "ConsoleArguments.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Defines a handler with function_name, &function_name will be a pointer to the function.
 */
#define DEFINE_HANDLER(function_name) size_t (function_name) (struct ConsoleArgumentSettings* _CAS, size_t argc, const char** argv, size_t index, size_t index_offset, enum RETURNVAL* ret)
/**
 * @brief Indicates the start of a argument parser function. Should be defined with DEFINE_HANDLER(function_name) above ArgTable.
 */
#define BEGIN_ARG_PARSER(function_name) size_t (function_name) (struct ConsoleArgumentSettings* _CAS, size_t argc, const char** argv, size_t index, size_t index_offset, char* ret){ \
		if(index + 1 + index_offset >= argc){*ret = TOO_FEW_ARGS; return 0;}
/**
 * @brief Indicates the end of an argument parser function.
 */
#define END_ARG_PARSER return index_offset; } 
/**
 * @brief Indicates the start of a flag parser function. Should be defined with DEFINE_HANDLER(function_name) above ArgTable.
 */
#define BEGIN_FLAG_PARSER(function_name) size_t (function_name) (struct ConsoleArgumentSettings* _CAS, size_t argc, const char** argv, size_t index, size_t index_offset, char* ret){
/**
 * @brief Indicates the end of an argument parser function.
 */
#define END_FLAG_PARSER END_ARG_PARSER


// DEFINE_HANDLER(handle_example_flag);
/** USER CUSTOM COMMAND PREDIFINES START HERE **/

/** USER CUSTOM COMMAND PREDIFINES END HERE **/

const struct nlist ArgTable[] = {
	/**
	 * @brief Entry should contain in order:
	 * Fullname used for --fullname
	 * singlecharacter equivalent used for -c
	 * function handler defined by DEFINE_HANDLER() macro
	 * a brief description in format "-e, --example [spacing] what the argument does"
	 * an extended description for when --argument -h is used
	 */
	/* {"example", 'e', &handle_example_flag, "-e, --example shows an example argument", 
		"here comes a detailed explanation for when -h is used on the argument"}, */
/** USER VALUES FOR ARGUMENT TABLE START HERE **/
	


/** USER VALUES FOR ARGUMENT TABLE END HERE **/
};


/** 
 * NOTICE: A Parser function has access to the following arguments
 * If any argument values are parsed, as happens in a BEGIN_ARG_PARSER function, the index_offset should be increased by the amount of arguments parsed.
 * 
 * @param _CAS a pointer to the ConsoleArgumentSettings struct in which to put parsed values. (struct defined in .h)
 * @param argc the amount of arguments passed from command line
 * @param argv the values of the arguments passed from command line
 * @param index the current argument in argv we're handling
 * @param index_offset used for argument chaining, e.g. -abc to indicate which argument in argv is associated with the current argument 
 * @param ret return buffer to indicate the state of the handler, by default this value should be set to 0 to indicate no error. 
 * 			  If the argument parser should exit after the argument is parsed set this to RET_AFTER_DONE
 */


/** flag example
BEGIN_FLAG_PARSER(handle_example_flag)
 	_CAS->val = 1;
 	*ret = NORMAL_COND;
END_FLAG_PARSER()
*/

/** argument parser example
BEGIN_ARG_PARSER(handle_example_arg)
 	_CAS->val = argv[index + 1 + index_offset++]; // <- increases index_offset for the extra argument parsed.
 *ret = NORMAL_COND;
END_ARG_PARSER
*/
/** CUSTOM HANDLERS START HERE **/





/** CUSTOM HANDLERS END HERE **/






















// Don't touch anything from this point on
/**
 * @brief Indicates the size of the ArgumentTable
 * 
 */
static size_t ARGTABLESIZE = sizeof(ArgTable)/sizeof(ArgTable[0]);

/**
 * @brief Struct containing set argument values
 * 
 */
struct ConsoleArgumentSettings _CAS;

/**
 * @brief Gets the Console Argument Settings object
 * 
 * @param CAS the values gotten from the console arguments
 */
void getConsoleArgumentSettings(struct ConsoleArgumentSettings* CAS){
	CAS = &_CAS;
}

/**
 * @brief Struct containing version
 * 
 */
struct Version _Version;

/**
 * @brief Get the Software Version object
 * 
 * @param _Version the software version struct
 */
void getSoftwareVersion(struct Version* _V){
	_V = &_Version;
}

/**
 * @brief Handles console arguments
 * 
 * @param argc the amount of console arguments
 * @param argv the argument values
 * @return RETURNVAL return value with errors
 */
enum RETURNVAL HandleConsoleArgs(const int argc, const char **argv){
	if (argc < 1 + HAS_REQUIRED_ARGUMENTS) {
        printf("Not enough arguments, Use -h for help\n");
        return TOO_FEW_ARGS;
    }

	enum RETURNVAL ret_val = NORMAL_COND;

	for(int i = 1; i < argc; i++){
		const char* text = argv[i];
		size_t size = strlen(text);
		if(size >= 2){
			// handle full string
			if(text[0] == text[1] && text[0] == '-'){
				// handle --options
				text+=2; // skip the two --
				size_t skipargs = HelpFullString(text, argv, i, argc, &ret_val);
				i+= skipargs;
			}
			else{
				if(text[0] == '-'){
					text++; // skip -
					size_t skip_args = HelpSingleChar(text, argv, i, argc, &ret_val, 0);
					i += skip_args;
				} 
				else{ // command did not start with - error
					printf("unknown console argument, use -h for help\n");
					return UNKOWN_ARG;
				}
			}
		}
		if(ret_val){
			return ret_val; // either error or 
		}
	}

	return NORMAL_COND;
}

/**
 * @brief Parses fullstring arguments preceded with --
 * 
 * @param text the argument to be parsed
 * @param argv the full set of arguments
 * @return size_t the amount of arguments parsed
 */
size_t HelpFullString(const char* text, const char** argv, size_t index, const size_t argc, enum RETURNVAL* ret_val){
	char converted_single_char = '\0';
	char should_printhelp = '\0';
	if(argc > index + 1){ // there is another argument after this one ( check if it's help )
		size_t next_arg_str_len = strlen(argv[index+1]);
		if(next_arg_str_len > 2 && argv[index+1][0] == argv[index+1][1] && argv[index+1][0] == '-'){
			// full string
			if(strcmp(argv[index+1], "help")){
				should_printhelp = 1;
			}
		}
		else{
			if(next_arg_str_len >= 2 && argv[index+1][0] == '-'){
				// single character
				if(argv[index+1][1] == 'h'){
					should_printhelp = 1;
				}
			}
		}
	}
	if(!strcmp(text, "help")){
		converted_single_char = 'h';
	}
	if(!strcmp(text, "version")){
		converted_single_char = 'v';
	}
	// CUSTOM ARGUMENTS START HERE
	for(size_t i = 0; i < ARGTABLESIZE; i++){
		if(!strcmp(text, ArgTable[i].fulltext)){
			converted_single_char = ArgTable[i].associatedchar;
			break;
		}
	}
	// CUSTOM ARGUMENTS END HERE
	if(should_printhelp){
		printHelp(argv[0], converted_single_char);
		*ret_val = RET_AFTER_DONE; // return after printing help
		return 1; // we parsed the next -h command already
	}
	if(converted_single_char){
		return HelpSingleCharParse(converted_single_char, argv, index, argc, ret_val, index, text); // parse the single character version of it
	}
	else{
		printf("Unknown argument: --%s\n", text);
		*ret_val = RET_AFTER_DONE;
		return 0; // we didn't parse anything
	}
}

/**
 * @brief does the recursion part of single char argument parsing
 * Can handle multiple characters with 1 - e.g. -abcde
 * @param text the argument to be parsed
 * @param argv the full set of argument values
 * @param index the argument index that is currently being parsed
 * @param argc the full amount of arguments
 * @param ret_val buffer that will containing resulting code for last executed argument
 * @param index_offset used for recursion, indicates the position of the next value associated with a possible argument
 * @return size_t the amount of arguments parsed
 */
size_t HelpSingleChar(const char* text, const char** argv, size_t index, const size_t argc, enum RETURNVAL* ret_val, size_t index_offset){
	char should_printhelp = '\0';
	if(argc > index + 1){ // there is another argument after this one ( check if it's help )
		size_t next_arg_str_len = strlen(argv[index+1]);
		if(next_arg_str_len > 2 && argv[index+1][0] == argv[index+1][1] && argv[index+1][0] == '-'){
			// full string
			if(strcmp(argv[index+1], "help")){
				should_printhelp = 1;
			}
		}
		else{
			if(next_arg_str_len >= 2 && argv[index+1][0] == '-'){
				// single character
				if(argv[index+1][1] == 'h'){
					should_printhelp = 1;
				}
			}
		}
	}
	
	// check how many characters we have
	size_t text_size = strlen(text);
	if(text_size > 0){ // there is atleast 1 char
		if(should_printhelp){
			printHelp(argv[0], text[0]);
			*ret_val = RET_AFTER_DONE;
			return index_offset + 1;
		}

		// index_offset contains the new starting position for arguments associated with the argument
		index_offset = HelpSingleCharParse(text[0], argv, index, argc, ret_val, index_offset, ""); 
		if(text_size == 1 || *ret_val == RET_AFTER_DONE){ // last argument handled
			return index_offset; // the amount of arguments that were parsed
		}
	}
	if(text_size > 1){ // there are more chars to be parsed
		text++;
		return HelpSingleChar(text, argv, index, argc, ret_val, index_offset);
	}	
	*ret_val = NO_ARGS_PARSED;
	return index_offset;
}	

/**
 * @brief parses single character arguments, text should not contain -
 * 
 * @param text the argument to be parsed
 * @param argv the full set of argument values
 * @param index the argument index that is currently being parsed
 * @param argc the full amount of arguments
 * @param ret_val buffer that will containing resulting code for last executed argument
 * @param index_offset used for recursion, indicates the position of the next value associated with a possible argument
 * @param fullargname used for errors when called from HelpFullString()
 * @return size_t the amount of arguments parsed
 */
size_t HelpSingleCharParse(const char text, const char** argv, const size_t index, const size_t argc, enum RETURNVAL* ret_val, size_t index_offset, const char* fullargname){
	size_t i;
	char ret;
	switch (text){
		case 'h':
			printHelp(argv[0], '\0');
			*ret_val = RET_AFTER_DONE;
			break;
		case 'v':
			printVersion(&_Version);
			*ret_val = RET_AFTER_DONE;
			break;
		default:
			for(i = 0; i < ARGTABLESIZE; i++){
				if(ArgTable[i].associatedchar == text){ // check if the text is in the table
					index_offset = ArgTable[i].handle_function(&_CAS, argc, argv, index, index_offset, &ret); // execute custom function
					if(ret == TOO_FEW_ARGS){ // check feedback
						printTooFewArguments(argv[0], text, fullargname);
					}
					break;
				}
			}
			if(i == ARGTABLESIZE){
				printf("argument not recognised\n");
				index_offset++; // so we don't get infinite loops from recursive parent call
				*ret_val = NO_ARGS_PARSED;
			}
			break;
		}
	return index_offset;
}

// standard commands:
void printHelp(const char* programname, const char extended_help){
	if(!extended_help){
		printf("Usage: %s [OPTION...] [VALUE]\n", programname);
		printf("Default usage: %s -r <record_time>\n", programname);
		printf("\t-h, --help for this message\n");
		printf("\t-v, --version for version\n");
		for(size_t i = 0; i < ARGTABLESIZE; i++){
			if(ArgTable[i].associatedchar == extended_help){
				printf("\n");
				printf(ArgTable[i].briefhelp_description, programname); // print help
				printf("\n");
			}
		}
		return;
	}
	switch (extended_help)
	{
	case 'h':
		printf("print's info about the program and arguments.\n");
		break;
	case 'v':
		printf("print's the current running software version.\n");
		break;
	default:
		for(size_t i = 0; i < ARGTABLESIZE; i++){
			if(ArgTable[i].associatedchar == extended_help){
				printf(ArgTable[i].extendedhelp_description, programname); // print extended help
				printf("\n");
				break;
			}
		}
		break;
	}
}

void printTooFewArguments(const char* programname, const char argument, const char* fullargname){
	printf("Error too few arguments for argument");
	if(strlen(fullargname) > 0){
		printf(" --%s\n", fullargname);
	}
	else{
		printf(" -%c\n", argument);
	}
	printHelp(programname, argument);
}

void printVersion(const struct Version *V){
	printf("Current version %d.%d.%d\n", V->Major, V->Minor, V->Bugfix);
}