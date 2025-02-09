#ifndef _DEBUG_H
#define _DEBUG_H

#include <string>

// #define DEBUG
#define DEBUG_INTERNAL
// #define DEBUG_SENSORS

///////
/// char[]
///////
extern void debug(char* str);

extern void debug(char* str, char value[]);

extern void debug(char* str, double value);

extern void debug(char* str, float value);

extern void debug(char* str, long value);

extern void debug(char* str, int value);

extern void debug(char* str, unsigned long value);

extern void debug(char* str, unsigned int value);

///////
/// std::string
///////
extern void debug(std::string str[]);

extern void debug(std::string str[], double value);

extern void debug(std::string str[], float value);

extern void debug(std::string str[], long value);

extern void debug(std::string str[], int value);

extern void debug(std::string str[], unsigned long value);

extern void debug(std::string str[], unsigned int value);

extern void debug(std::string str[], std::string value[]);

#endif