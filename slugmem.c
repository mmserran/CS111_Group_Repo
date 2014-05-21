
//#include "slugmem.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>



struct allocation
{
	void* loc;
	char* where;
	time_t when;
	size_t size;
	bool active;
};

struct hashtable
{
	size_t size;
	size_t load;
	struct allocation** table;
	int active_entries;
};

struct hashtable* ht = NULL;
int EXIT_STATUS = 0;


void destroy_hashtable()
{
	int i;
	for(i = 0; i < ht->size; ++i)
	{
		if(ht->table[i] != NULL)
		{
		    free(ht->table[i]);
		    ht->table[i] = NULL;
		}

	}
	free(ht->table);
	free(ht);
}
/*
void double_hashtable()
{
	//ht = malloc(sizeof(struct hashtable));
	struct allocation** oldtable = ht->table;
	size_t oldsize = ht->size;

	ht->size = (oldsize * 2) + 1;
	ht->table = calloc(ht->size, sizeof(struct allocation*));

	for(int i = 0; i < oldsize; ++i){
		if(oldtable[i] != NULL)
			ht->table[]
	}

	ht->load = 0;
	atexit(slug_memstats);		
}
*/
void insert_into_hashtable(void* loc, char* where, time_t when, size_t size)
{
	//assert(ht->size != ht->load + 5);
	size_t pos = (unsigned long) loc % ht->size;
	while(ht->table[pos] != NULL)
	{
		pos = (pos + 1) % ht->size;
	}
	struct allocation* a = malloc(sizeof (struct allocation));

	a->loc = loc;
	a->where = where;
	a->when = when;
	a->size = size;
	a->active = true;

	ht->table[pos] = a;
	//++ht->load;
	++ht->active_entries;
}

struct allocation* is_in_hashtable(void* loc)
{
	size_t pos = (unsigned long) loc % ht->size;
	while(ht->table[pos] != NULL)
	{
		if (ht->table[pos]->loc == loc) return ht->table[pos];

		pos = (pos + 1) % ht->size;

	}
	return NULL;
}


void slug_memstats ( void )
{
	if(ht == NULL){
		printf("There were no allocations!\n");
		exit(0);
	}

	printf("Memory Dump:\n\n ================================================ \n\n");

	int i;
	struct tm * timeinfo;
	for(i = 0; i < ht->size; ++i)
	{
		if(ht->table[i] == NULL)
			continue;

		timeinfo = localtime(&ht->table[i]->when);


		printf("%30s: %d\n", "table entry", i);
		printf("%30s: %p\n", "allocated memory address", ht->table[i]->loc);
		printf("%30s: %zu\n", "allocation size", ht->table[i]->size);
		printf("%30s: %s\n", "time of allocation", asctime(timeinfo));

	}
		
	printf(" ================================================ \n\n");

	if(EXIT_STATUS == 1)
		printf("There were problems.\n");

	if(EXIT_STATUS == 0)
		printf("There were no problems.\n");

	destroy_hashtable();
}


void slug_free ( void *addr, char *where )
{
	//printf("%p\n", addr);

	if(ht == NULL)
	{ 
		fprintf(stderr, "\nERROR: Memory not allocated\n\n %15s: %s\n %15s: %p\n\n",
			"at", where, "target memory", addr);
		EXIT_STATUS = 1;
		exit(0);
	}
	struct allocation* a = is_in_hashtable(addr);

	if(a == NULL)
	{
		fprintf(stderr, "\nERROR: Memory not allocated\n\n %15s: %s\n %15s: %p\n\n",
			"at", where, "target memory", addr);
		EXIT_STATUS = 1;
		exit(0);
	}

	if(a->active == false)
	{
		fprintf(stderr, "Error. Memory already freed at %s\n", where);
		EXIT_STATUS = 1;

		exit(0);
	}
	a->active = false;

	free(a->loc);

}

void init_hashtable()
{
	ht = malloc(sizeof(struct hashtable));
	ht->table = calloc(2049, sizeof(struct allocation*));
	ht->size = 2049;
	ht->load = 0;
	atexit(slug_memstats);
}


void* slug_malloc(size_t size, char* where)
{
	if (ht == NULL)
		init_hashtable();

	if(size == 0) 
		fprintf(stderr, "%s:Unusual Operation.  Allocation of size 0.\n", where);

	time_t now;
	time(&now);

	void* loc = malloc(size);
	insert_into_hashtable(loc, where, now, size);
	return loc;
}


