#ifndef READCONFIG_H
#define READCONFIG_H

#define MAX_LINE_WIDTH  512
#define COMMENT_CHAR    '#'

#define MAX_CONFIG_ENTRIES  5
#define ASCII_MIN_RANGE 46
#define ASCII_MAX_RANGE 126

//#define DEBUG_VERBOSE

int parseConfig(const char* fileName);

const char* getConfigValue(const char* valueName);

const char* getParseError();

#endif