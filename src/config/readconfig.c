#include "readconfig.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int parseConfigErr;
static int configLoaded = 0;

enum parseConfigErrors {
    PC_OK = 0,
    PC_CANTOPEN,
    PC_PARSEERROR,
    PC_NOCONFIG,
    PC_NOVAL
};

enum parseConfigTypes {
    PCT_INT,
    PCT_FLOAT,
    PCT_STRING
};

struct configEntry {
    char* entryName;
    enum parseConfigTypes entryType;
    void *entryData;
};

static struct configEntry *entries[MAX_CONFIG_ENTRIES];

static struct configEntry* parseConfigLine(const char* line, size_t len) {
    int i = 0;
    int status = 0;
    //0 = searching for entry
    //1 = found name
    //2 = searching for equal
    //3 = searching for value
    //4 = reading value
    int index;
    char name[MAX_LINE_WIDTH];
    char value[MAX_LINE_WIDTH];
    for(i = 0; i < len; i++) {
        switch (status) {
            case 0:
                if(line[i] == COMMENT_CHAR) {
                    //printf("Comment found :)\n");
                    return NULL;
                } else if (line[i] >= ASCII_MIN_RANGE && line[i] <= ASCII_MAX_RANGE){
                    //found entry
                    index = i;
                    status = 1;
                }
                break;
            case 1:
                if(line[i] == COMMENT_CHAR || line[i] == '\n') {
                    parseConfigErr = PC_PARSEERROR;
                    return NULL;
                } else if (line[i] < ASCII_MIN_RANGE || line[i] > ASCII_MAX_RANGE) {
                    strncpy(name, &(line[index]), i - index);
                    name[i - index] = '\0';
                    #ifdef DEBUG_VERBOSE
                    printf("Identified name as: [%s]\n", name);
                    #endif
                    status = 2;
                }
                break;
            case 2:
                if(line[i] == '=')
                    status = 3;
                break;
            case 3:
                if (line[i] >= ASCII_MIN_RANGE && line[i] <= ASCII_MAX_RANGE){
                    //found entry
                    index = i;
                    status = 4;
                }
                break;
            case 4:
                if (line[i] < ASCII_MIN_RANGE || line[i] > ASCII_MAX_RANGE || line[i] == COMMENT_CHAR) {
                    //printf("index: %d\ti: %d\n", index, i);
                    strncpy(value, &(line[index]), i - index);
                    value[i - index] = '\0';
                    #ifdef DEBUG_VERBOSE
                    printf("Identified value as: [%s]\n", value);
                    #endif
                    status = 5;
                }
                break;
            /*default:
                parseConfigErr = PC_PARSEERROR;
                return NULL;
                break;
            */
        }
    }
    if (status == 4) { //if EOF
        strncpy(value, &(line[index]), i - index);
        value[i - index] = '\0';
        #ifdef DEBUG_VERBOSE
        printf("Identified value as: [%s]\n", value);
        #endif
        status = 5;
    }
    if (status < 4 && status > 0) {
        parseConfigErr = PC_PARSEERROR;
        return NULL;
    }
    struct configEntry* ret = malloc(sizeof(struct configEntry));
    ret->entryName = malloc(sizeof(char) * MAX_LINE_WIDTH);
    ret->entryData = malloc(sizeof(char) * MAX_LINE_WIDTH);
    strcpy(ret->entryName, name);
    strcpy(ret->entryData, value);
    ret->entryType = PCT_STRING;

    return ret;
}

int parseConfig(const char* fileName) {
    FILE *f;
    f = fopen(fileName, "r");
    if (f == NULL) {
        parseConfigErr = PC_CANTOPEN;
        return parseConfigErr;
    }

    int i;
    for (i = 0; i < MAX_CONFIG_ENTRIES; i++)
        entries[i] = NULL;
    i = 0;

    char buffer[MAX_LINE_WIDTH];
    char *line = buffer;
    size_t len;
    size_t line_len;
    
    line_len = getline(&line, &len, f);
    while(line_len != -1) {
        printf("This line is %zu long.\n", line_len);
        struct configEntry* e = parseConfigLine(line, line_len);
        if(e == NULL && parseConfigErr != PC_OK)
            return parseConfigErr;
        if(e) {
            entries[i] = e;
            i++;
        }
        line_len = getline(&line, &len, f);
    }
    fclose(f);
    configLoaded = 1;
    return PC_OK;
}

const char* getConfigValue(const char* valueName) {
    int i = 0;
    int found = 0;
    if(configLoaded != 1) {
        parseConfigErr = PC_NOCONFIG;
        return NULL;
    }
    for (i = 0; i < MAX_CONFIG_ENTRIES; i++) {
        if(entries[i]) {
            if (strcmp(valueName, entries[i]->entryName)==0) {
                found = 1;
                break;
            }
        }
    }
    if (found == 0) {
        parseConfigErr = PC_NOVAL;
        return NULL;
    }
    return entries[i]->entryData;
}

const char* getParseError() {
    switch (parseConfigErr) {
        case PC_OK:
            return "No error parsing config.";
            break;
        case PC_CANTOPEN:
            return "Cannot open config file for parsing.";
            break;
        case PC_PARSEERROR:
            return "Error parsing config file.";
            break;
        case PC_NOCONFIG:
            return "No config has been loaded.";
            break;
        case PC_NOVAL:
            return "Value not found in the config file.";
            break;
        default:
            return "Unknown config parsing error.";
            break;
    }
}