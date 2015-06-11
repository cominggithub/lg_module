#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "conf_util.h"
#include "dbg_log.h"

#define VAR_SIZE 255
#define BUF_SIZE 255

//const char CONF_FILE[]="conf/conf.txt";

static char varName[VAR_SIZE][BUF_SIZE];
static char varValue[VAR_SIZE][BUF_SIZE];
static int varCount;

static int get_var_index(const char* name);
static int getVarName(char* buf, char *name);
static int getValue(char* buf, char *value);
static int load_var(FILE* fp);


double conf_getVarDouble(const char* name)
{
	int index;
	index = get_var_index(name);

	if (index == -1)
		return 0.0;
	return (double)atof(varValue[index]);
}


char* conf_getVarStr(const char* name)
{
	int index;
	index = get_var_index(name);

	if (index == -1)
		return "";
	return varValue[index];
}

long int conf_getVarInt(const char* name)
{
	int index;
	index = get_var_index(name);

	if (index == -1)
		return 0;
	return atol(varValue[index]);

}

int conf_load(const char* fileName)
{
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	size_t read;
	char buf[BUF_SIZE];
	double dd=12.3;

	fp = fopen(fileName, "r");
	pStr(fileName);
	if (fp == NULL)
	{
		fprintf(stderr, "cannot read config file\n");
		return 0;
	}

	load_var(fp);
//	conf_dump();
	fclose(fp);
	return 1;
}

void conf_dump()
{
	int i=0;
	for(i=0; i<varCount; i++)
	{
		printf("{%s = %s}\n", varName[i], varValue[i]);
	}
}
int getVarName(char* buf, char *name)
{
	int index;
	char* e;

	e = strchr(buf, '=');

	index = (int)(e - buf);
	if (index < 1)
		return 0;

	strncpy(name, buf, index);
	name[index]='\0';

	return 1;

}
int getValue(char* buf, char *value)
{
	int index;
	char* e;
	char v[100];


	e = strchr(buf, '=');
	index = (int)(e - buf);
	if (index < 1)
		return 0;

	strncpy(value, buf+index+1, strlen(buf)-index-1);
	value[strlen(buf)-index-1]='\0';
	return 1;
}

bool isPrintable(const char* str)
{
	int i;
	for(i=0; i<strlen(str); i++)
	{
  		if (isprint(str[i]) && str[i] != ' ')
  		{
  			return true;
  		}
    }

  	return false;
}
int load_var(FILE* fp)
{
	char buf[BUF_SIZE*2];
	int charCnt;

	varCount = 0;
	while (fgets(buf, BUF_SIZE, fp) != NULL)
	{

		if (buf[strlen(buf)-1] == '\n')
		{
			buf[strlen(buf)-1]='\0';
		}

		if (buf[strlen(buf)-1] == '\r')
		{
			buf[strlen(buf)-1]='\0';
		}

		if (strlen(buf) < 1)
		{
			continue;
		}
		if (!isPrintable(buf))
		{
			continue;
		}

		if (buf[0] == '/' && buf[1] == '/')
		{
			continue;
		}

		if (getVarName(buf, varName[varCount]) != 0 &&
			getValue(buf, varValue[varCount]) != 0)
		{
			varCount++;
		}
	}

	return 1;
}


int get_var_index(const char* name)
{
	int i;
	for(i=0; i<varCount; i++)
	{
		if(strcmp(name, varName[i]) == 0)
		{
			return i;
		}
	}

	return -1;
}

