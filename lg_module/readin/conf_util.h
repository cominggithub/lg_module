#include <stdio.h>

int conf_load(const char* fileName);
char* conf_getVarStr(const char* name);
int conf_getVarInt(const char* name);
float conf_getVarFloat(const char* name);
void conf_dump();