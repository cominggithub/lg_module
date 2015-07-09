#include <stdio.h>

int conf_load(const char* fileName);
char* conf_getVarStr(const char* name);
long int conf_getVarInt(const char* name);
double conf_getVarDouble(const char* name);
void conf_dump();