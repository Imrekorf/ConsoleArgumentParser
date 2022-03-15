#include "ConsoleArguments.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define RET_AFTER_DONE 10
#define TOO_FEW_ARGS 20
#define NO_ARGS_PARSED 30

/** USER CUSTOM COMMAND PREDIFINES START HERE **/

// size_t handle_xxxx_arg	(struct ConsoleArgumentSettings* _CAS, size_t argc, const char** argv, size_t index, size_t index_offset, char* ret);

/** USER CUSTOM COMMAND PREDIFINES END HERE **/
/** USER VALUES FOR ARGUMENT TABLE START HERE **/

const struct nlist ArgTable[] = {
	// {"example", 'e', &handle_xxxx_arg, "-e, --example shows an example argument", "here comes a detailed explanation for when -h is used on the argument"},
	


/** USER VALUES FOR ARGUMENT TABLE END HERE **/
	{"", 0, nullptr, ""} // required ended of the table
};


/** CUSTOM HANDLERS START HERE **/

// flag example
// size_t handle_xxxx_arg	(struct ConsoleArgumentSettings* _CAS, size_t argc, const char** argv, size_t index, size_t index_offset, char* ret){
// 	_CAS->val = 1;
// 	*ret = 0;
// 	return index_offset; // mandatory
// }

// argument parser example
// size_t handle_xxxx_arg		(struct ConsoleArgumentSettings* _CAS, size_t argc, const char** argv, size_t index, size_t index_offset, char* ret){
 	// example of argument count check 1 = amount of arguments ( not necessary if it's a flag )
// 	if(index + 1 + index_offset >= argc){
// 		*ret = TOO_FEW_ARGS;
// 		return 0;	
// 	}
// 	_CAS->val = argv[index + 1 + index_offset++];
// 	return index_offset; // mandatory
// }

/** CUSTOM HANDLERS END HERE **/

void printHelp(const char* programname, const char extended_help){
	if(!extended_help){
		printf("Usage: %s [OPTION...] [VALUE]\n", programname);
		printf("Default usage: %s -r <record_time>\n", programname);
		printf("\t-h, --help for this message\n");
		printf("\t-v, --version for version\n");
		int i = 0;
		handle_arg_template func = ArgTable[i].handle_function;
		while(func){
			if(ArgTable[i].associatedchar == extended_help){
				printf("\n");
				printf(ArgTable[i].briefhelp_description, programname); // print help
				printf("\n");
			}
			func = ArgTable[++i].handle_function;
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
		int i = 0;
		handle_arg_template func = ArgTable[i].handle_function;
		while(func){
			if(ArgTable[i].associatedchar == extended_help){
				printf(ArgTable[i].extendedhelp_description, programname); // print extended help
				printf("\n");
				break;
			}
			func = ArgTable[++i].handle_function;
		}
		break;
	}
}















// Don't touch anything from this point on

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
 * @return int return value with errors
 */
int HandleConsoleArgs(const int argc, const char **argv){
	if (argc < 2) {
        printf("Not enough arguments, Use -h for help\n");
        return -2;
    }

	char ret_val = 0;

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
					size_t skip_args = HelpSingleChar(text, argv, i, argc, &ret_val);
					i += skip_args;
				} 
				else{ // command did not start with - error
					printf("unknown console argument, use -h for help\n");
					return -1;
				}
			}
		}
		if(ret_val){
			return ret_val;
		}
	}

	return 0;
}

/**
 * @brief Parses fullstring arguments preceded with --
 * 
 * @param text the argument to be parsed
 * @param argv the full set of arguments
 * @return size_t the amount of arguments parsed
 */
size_t HelpFullString(const char* text, const char** argv, size_t index, const size_t argc, char* ret_val){
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
	int i = 0;
	handle_arg_template func = ArgTable[i].handle_function;
	while(func){
		if(!strcmp(text, ArgTable[i].fulltext)){
			converted_single_char = ArgTable[i].associatedchar;
			break;
		}
		func = ArgTable[++i].handle_function;
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
size_t HelpSingleChar(const char* text, const char** argv, size_t index, const size_t argc, char* ret_val, size_t index_offset){
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
		index_offset = HelpSingleCharParse(text[0], argv, index, argc, ret_val, index_offset); 
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
size_t HelpSingleCharParse(const char text, const char** argv, const size_t index, const size_t argc, char* ret_val, size_t index_offset, const char* fullargname){
	switch (text){
		case 'h':
			printHelp(argv[0]);
			*ret_val = RET_AFTER_DONE;
			break;
		case 'v':
			printVersion(&_Version);
			*ret_val = RET_AFTER_DONE;
			break;
		default:
		int i = 0;
			handle_arg_template func = ArgTable[i].handle_function;
			while(func){
				if(ArgTable[i].associatedchar == text){ // check if the text is in the table
					char ret;
					index_offset = ArgTable[i].handle_function(&_CAS, argc, argv, index, index_offset, &ret); // execute custom function
					if(ret == TOO_FEW_ARGS){ // check feedback
						printTooFewArguments(argv[0], text, fullargname);
					}
					break;
				}
				func = ArgTable[++i].handle_function;
			}
			if(!func)
				printf("argument not recognised\n");
				index_offset++; // so we don't get infinite loops from recursive parent call
				*ret_val = NO_ARGS_PARSED;
			break;
		}
	return index_offset;
}

// standard commands:

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