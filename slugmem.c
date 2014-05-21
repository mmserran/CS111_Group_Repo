
//#include "slugmem.h"
/*  CREATED 5/21/2014
 *  Group members: Tyler Smith, James Shedel, Mark Serrano, James Kuch, Richard Nicholson
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>

struct allocation
{
	void* loc;   //value returned by malloc
	char* where; //location within the file 
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

/* ----------destroy_hashtable()---------
 * @params: none
 * @precondition: table must exist
 * @postcondition: table has ceased to exist
 *
 * description: destroys the hash table and frees the memory that was used by it
 * -------------------------------------- */

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

/* ---------insert_into_hashtable()------
 * @params: loc (the memory location returned by malloc), where (line number in file), when (time allocated),
 * size (size of allocation)
 * @precondition(s): none of the params are null
 * @postcondition: a new allocation data structure a is in the hash table
 *
 * description: takes arguments, allocates memory for the allocation hash table data structure, and 
 * then inserts the data structure into the table at position: loc % (size of hash table)
 * -------------------------------------- */

void insert_into_hashtable(void* loc, char* where, time_t when, size_t size)
{
	//assert(ht->size != ht->load + 5); //temporary
	size_t pos = (unsigned long) loc % ht->size;

	/* this is linear probing, if the space we want to use
	 * is already occupied by another allocation, we add one to the pos
	 * and re-hash the position */
	while(ht->table[pos] != NULL)
	{
		pos = (pos + 1) % ht->size;
	}

	//Create the allocation data structure and fill it with the paramaters
	struct allocation* a = malloc(sizeof (struct allocation));
	a->loc = loc;
	a->where = where;
	a->when = when;
	a->size = size;
	a->active = true;

	//Insert the allocation structure into the hash table
	ht->table[pos] = a;
	++ht->load;
	++ht->active_entries;
}

/* ----------is_in_hashtable()-----------
 * @params: loc -> memory location of element to look up in hash table
 * @precondition: hash table must exist, loc must not be null
 * @postcondition: returns null if loc is not in hashtable
 *                 returns pointer to allocation structure if loc is in hashtable
 *
 * description: takes memory location as argument, checks to see if the given location is located in
 * the hash table by hashing the memory location and seeing if there is data at that position. if there 
 * is no data, then NULL is returned.
 * -------------------------------------- */

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
	if(ht == NULL) {
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


