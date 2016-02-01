#include "time_util.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "dbg_log.h"

static char sname[1024][1024];
static int used_time_index = 0;

static clock_t start_time[256];
static clock_t end_time[256];

static time_t start_time_t[256];
static time_t end_time_t[256];

static double elapsed_time[256];
/* here, do your time-consuming job */

void add_elapsed_time(const char *name);


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
	int index;
	index = find_name_index((name));
	if ( index == -1)
	{
		strcpy(sname[used_time_index], name);
		start_time[used_time_index] = clock();
		end_time[used_time_index] 	= clock();

		start_time_t[used_time_index] 	= time(NULL);
		end_time_t[used_time_index] 	= time(NULL);
		elapsed_time[used_time_index] 	= 0;
		used_time_index++;
	}
	else
	{
		start_time[index] 	= clock();
		start_time_t[index]	= time(NULL);
	}
}


void set_end_time(const char *name)
{
	int index;

	index = find_name_index(name);
	
	if (index == -1)
		return;
	end_time[index] = clock();
	end_time_t[index] = time(NULL);
	add_elapsed_time(name);
}

void add_elapsed_time(const char *name)
{
	int index;
	double time_spent;
	index = find_name_index(name);
	
	if (index == -1)
		return;

	// time_diff 			= difftime(end_time_t[index], start_time_t[index]);
	time_spent = (double)(end_time[index] - start_time[index]) / CLOCKS_PER_SEC;
	// pD(time_spent);
	// pD(elapsed_time[index]);
	elapsed_time[index] += time_spent;
}

void set_end_time_and_add_elpased_time(char const *name)
{
	int index;

	index = find_name_index(name);
	
	if (index == -1)
		return;
	end_time[index] 	= clock();
	end_time_t[index] 	= time(NULL);
	
}



void print_all_execution_time()
{
	int i;
	for(i=0; i<used_time_index; i++)
	{
		print_execution_time(sname[i]);
	}
}

char* get_time_str(int seconds)
{

	char *str;
	str = new char[30];
	if (seconds < 60)
	{
		sprintf(str, "        %2d seconds", seconds);
	}
	else
	{
		sprintf(str, "%3d min %2d seconds", seconds/60, seconds%60);
	}
	return str;
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

	// printf("%s\n", get_time_str(59));
	// printf("%s\n", get_time_str(61));
	// printf("%s\n", get_time_str(119));
	// printf("%s\n", get_time_str(12000));
	time_spent = (double)(end_time[index] - start_time[index]) / CLOCKS_PER_SEC;
	// printf("%d [%30s] execution time: %10.6f(%10.6f) second\n", index, name, time_spent, difftime(end_time_t[index], start_time_t[index]));
	printf("%d [%25s] execution time: %s (%s)\n", 
		index, 
		name, 
		get_time_str(difftime(end_time_t[index], start_time_t[index])),
		get_time_str(elapsed_time[index])
		);
}

