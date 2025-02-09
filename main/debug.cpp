#include <stdio.h>

#include "constants.h"
#include "debug.h"

static const char* debugFormatStr = "%s\n";
static const char* debugFormatStrChar = "%s=%s\n";
static const char* debugFormatStrFloat = "%s=%f\n";
static const char* debugFormatStrInt = "%s=%d\n";
static const char* debugFormatStrLong = "%s=%ld\n";
static const char* debugFormatStrStr = "%s=%s\n";
static const char* debugFormatStrUnsignedInt = "%s=%du\n";
static const char* debugFormatStrUnsignedLong = "%s=%lu\n";

///////
/// char[]
///////
void debug(char* str) {
#if defined(DEBUG) || defined(DEBUG_INTERNAL)
    printf(debugFormatStr, str);
#endif
}

void debug(char* str, char value[]) {
#if defined(DEBUG) || defined(DEBUG_INTERNAL)
    printf(debugFormatStrChar, str, value);
#endif
}

void debug(char* str, double value) {
#if defined(DEBUG) || defined(DEBUG_INTERNAL)
    printf(debugFormatStrFloat, str, value);
#endif
}

void debug(char* str, float value) {
#if defined(DEBUG) || defined(DEBUG_INTERNAL)
    printf(debugFormatStrFloat, str, value);
#endif
}

void debug(char* str, int value) {
#if defined(DEBUG) || defined(DEBUG_INTERNAL)
    printf(debugFormatStrInt, str, value);
#endif
}

void debug(char* str, long value) {
#if defined(DEBUG) || defined(DEBUG_INTERNAL)
    printf(debugFormatStrLong, str, value);
#endif
}

void debug(char* str, unsigned int value) {
#if defined(DEBUG) || defined(DEBUG_INTERNAL)
    printf(debugFormatStrUnsignedInt, str, value);
#endif
}

void debug(char* str, unsigned long value) {
#if defined(DEBUG) || defined(DEBUG_INTERNAL)
    printf(debugFormatStrUnsignedLong, str, value);
#endif
}

///////
/// STRING
///////
void debug(std::string str) {
#if defined(DEBUG) || defined(DEBUG_INTERNAL)
    printf(debugFormatStr, str.c_str());
#endif
}

void debug(std::string str, double value) {
#if defined(DEBUG) || defined(DEBUG_INTERNAL)
    printf(debugFormatStrFloat, str.c_str(), value);
#endif
}

void debug(std::string str, float value) {
#if defined(DEBUG) || defined(DEBUG_INTERNAL)
    printf(debugFormatStrFloat, str.c_str(), value);
#endif
}

void debug(std::string str, int value) {
#if defined(DEBUG) || defined(DEBUG_INTERNAL)
    printf(debugFormatStrInt, str.c_str(), value);
#endif
}

void debug(std::string str, long value) {
#if defined(DEBUG) || defined(DEBUG_INTERNAL)
    printf(debugFormatStrLong, str.c_str(), value);
#endif
}

void debug(std::string str, std::string value) {
#if defined(DEBUG) || defined(DEBUG_INTERNAL)
    printf(debugFormatStrStr, str.c_str(), value.c_str());
#endif
}

void debug(std::string str, unsigned int value) {
#if defined(DEBUG) || defined(DEBUG_INTERNAL)
    printf(debugFormatStrUnsignedInt, str.c_str(), value);
#endif
}

void debug(std::string str, unsigned long value) {
#if defined(DEBUG) || defined(DEBUG_INTERNAL)
    printf(debugFormatStrUnsignedLong, str.c_str(), value);
#endif
}