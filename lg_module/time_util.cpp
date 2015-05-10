#include "time_util.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "dbg_log.h"

static char sname[1024][1024];
static int used_time_index = 0;

static clock_t start_time[256];
static clock_t end_time[256];

/* here, do your time-consuming job */



static int find_name_index(char const *name)
{
	int i;
	for(i=0; i<used_time_index; i++)
	{
		if (strcmp(name, sname[i]) == 0)
			return i;
	}
	
	return -1;
}

void set_start_time(char const *name)
{
	strcpy(sname[used_time_index], name);
	start_time[used_time_index] = clock();
	end_time[used_time_index] 	= clock();
	used_time_index++;
	
}

void set_end_time(char const *name)
{
	int index;

	index = find_name_index(name);
	
	if (index == -1)
		return;
	end_time[index] = clock();
}

void print_all_execution_time()
{
	int i;
	for(i=0; i<used_time_index; i++)
	{
		print_execution_time(sname[i]);
	}
}
void print_execution_time(char const *name)
{
	int index;
	double time_spent;
	index = find_name_index(name);
	if (index == -1)
	{
		printf("cannot find %s\n", name);
		return;	
	}

	time_spent = (double)(end_time[index] - start_time[index]) / CLOCKS_PER_SEC;
	printf("%d [%30s] execution time: %10.6f second\n", index, name, time_spent);
}