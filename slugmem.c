
//#include "slugmem.h"
/*  CREATED 5/21/2014
 *  Group members: Tyler Smith, James Shedel, Mark Serrano, James Kuch, Richard Nicholson
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>


/* Allocation data structure used in hash table */
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

void init_hashtable();
void destroy_hashtable();
void double_hashtable();
void insert_into_hashtable(struct allocation*);
struct allocation* is_in_hashtable(void* );



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

void double_hashtable()
{
	//ht = malloc(sizeof(struct hashtable));
	struct allocation** oldtable = ht->table;
	size_t oldsize = ht->size;

	ht->size = (oldsize * oldsize);
	ht->table = calloc(ht->size, sizeof(struct allocation*));
	int i = 0;
	for(i = 0; i < oldsize; ++i){
		if(oldtable[i] != NULL)
			insert_into_hashtable(oldtable[i]);
	}

	free(oldtable);
}


/* ---------insert_into_hashtable()------
 * @params: struct allocation*
 * @precondition(s): none of the params are null
 * @postcondition: a new allocation data structure a is in the hash table
 *
 * description: takes arguments, allocates memory for the allocation hash table data structure, and 
 * then inserts the data structure into the table at position: loc % (size of hash table)
 * -------------------------------------- */

void insert_into_hashtable(struct allocation* a)
{
	//assert(ht->size != ht->load + 5); //temporary
	if(ht->load * 4 > ht->size)
		double_hashtable();

	size_t pos = (unsigned long) a->loc % ht->size;

	/* this is linear probing, if the space we want to use
	 * is already occupied by another allocation, we add one to the pos
	 * and re-hash the position */
	while(ht->table[pos] != NULL)
	{
		pos = (pos + 1) % ht->size;
	}

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

/* -----------slug_memstats()-----------
 * @params: none
 * @precondition:
 * @postcondition: 
 * -------------------------------------- */

void slug_memstats ( void )
{
	if(ht == NULL) {
		printf("There were no allocations!\n");
		exit(0);
	}

	printf("Memory Dump:\n\n ================================================ \n\n");

	int i;
	struct tm * timeinfo;

	/* Loop through the hash table */
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

/* -----------slug_free()----------------
 * @params: addr -> the address of the memory region needing to be freed
 *			where -> the position in code where slug_free() is called 
 * @precondition: addr is not null, where is not null, ht has been created
 * @postcondition: the memory at addr is freed 
 *
 * description: this is the replacement for free(). it marks the memory in the hashtable as 
 * being inactive if it is in the hashtable and is active already.
 * -------------------------------------- */

void slug_free ( void *addr, char *where )
{
	//printf("%p\n", addr);

	/* check to see if the hashtable doesn't exist, in which case print out an error */
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

/* -----------slug_malloc()--------------
 * @params: size -> size of the memory region to be allocated
 *          where -> line number of memory allocation
 * @precondition: size and where are not null
 * @postcondition: memory is allocated and tracked in the hashtable
 * @returns: loc (allocated memory address)
 * 
 * description: replacement for malloc(). creates a hash table if not already instantiated, then allocates memory using 
 * malloc(), adds the memory address, timestamp, location within file and size of memory allocated
 * to the hasthtable for lookup later.
 * -------------------------------------- */

void* slug_malloc(size_t size, char* where)
{

	/* create the hashtable if this is the first malloc */
	if (ht == NULL)
		init_hashtable();

	/* special case if size of allocation is zero */
	if(size == 0) 
		fprintf(stderr, "%s:Unusual Operation.  Allocation of size 0.\n", where);

	time_t now;
	time(&now);

	void* loc = malloc(size);

	/* Create the allocation data structure and fill it with the paramaters */
	struct allocation* a = malloc(sizeof (struct allocation));
	a->loc = loc;
	a->where = where;
	a->when = now;
	a->size = size;
	a->active = true;

    /* add the location, line num, time and size of allocation to the hashtable */
	insert_into_hashtable(a);
	return loc;
}

/* -----------init_hashtable()-----------
 * @params: none
 * @precondition: none
 * @postcondition: creates a new hashtable of size 2049
 * -------------------------------------- */

void init_hashtable()
{
	ht = malloc(sizeof(struct hashtable));
	ht->table = calloc(2049, sizeof(struct allocation*));
	ht->size = 2049;
	ht->load = 0;

	/* run slug_memstats when the program exits */
	atexit(slug_memstats);
}

