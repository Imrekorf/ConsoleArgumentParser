Parses command line argument based on predefined values, set in ConsoleArguments.cpp's ```ArgTable```. In the table a full argument name is needed, a single character abbreviation, a function pointer to a handler ( custom defined ), and a detailed description.

Supports multiple single character arguments in one argument like -abcde</br>
Supports multiple multiple single character and fullname arguments aswell as mixed</br>
Supports multiple values on a single argument

after configuration the library can be used with
```C
int ret = HandleConsoleArgs(argc, argv);
if(ret){
	return 0; // return on specific functions
}
```
