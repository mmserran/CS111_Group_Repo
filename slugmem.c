
//#include "slugmem.h"
/*  CREATED 5/21/2014
 *  Group members: Tyler Smith, James Shedel, Mark Serrano, James Kuch, Richard Nicholson
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>


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

	/* clean up */
	free(ht->table);
	free(ht);
}

/* ----------double_hashtable()---------
 * @params: none
 * @precondition: table must exist
 * @postcondition: table is now twice the size
 *
 * description: doubles the size of the hashtable
 * -------------------------------------- */

void double_hashtable()
{
	//ht = malloc(sizeof(struct hashtable));
	/* save the old table */
	struct allocation** oldtable = ht->table;
	size_t oldsize = ht->size;

	/* create a new table that is twice the size using the same pointer */
	ht->size = (oldsize * oldsize);
	ht->table = calloc(ht->size, sizeof(struct allocation*));

	/* copy contents of old table into the new table */
	int i = 0;
	for(i = 0; i < oldsize; ++i){
		if(oldtable[i] != NULL)
			insert_into_hashtable(oldtable[i]);
	}

    /* clean up */
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
 * @precondition: none
 * @postcondition: none
 *
 * description: prints out the memory statistics for the program that ran, including:
 *              hash table entry
 *              allocated memory address
 *              allocation size
 *              time of allocation
 *              any errors, such as no memory allocations, invalid memory allocations/frees
 * -------------------------------------- */

void slug_memstats ( void )
{

	/* If the hash table does not exist, then there were no allocations */
	if(ht == NULL) {
		printf("There were no allocations!\n");
		exit(0);
	}

	printf("Memory Dump:\n\n ================================================ \n\n");

	struct tm * timeinfo;

	int n_allocs = 0;
	int total    = 0;

	double mean;
	double std_dev = 0;

	/* Loop through the hash table and print the data out */
	int i;
	for(i = 0; i < ht->size; ++i)
	{
		if(ht->table[i] == NULL)
			continue;

		timeinfo = localtime(&ht->table[i]->when);

		n_allocs += 1;
		total    += ht->table[i]->size;

		printf("%30s: %d\n", "table entry", i);
		printf("%30s: %p\n", "allocated memory address", ht->table[i]->loc);
		printf("%30s: %zu\n", "allocation size", ht->table[i]->size);
		printf("%30s: %s\n", "time of allocation", asctime(timeinfo));

	}
		
	printf(" ================================================ \n\n");

	/* Statistical Analysis - Mean and Standard Deviation*/
	mean = total/n_allocs;
	for(i = 0; i < ht->size; ++i)
	{
		if(ht->table[i] == NULL)
			continue;

		std_dev += pow(ht->table[i]->size - mean, 2.0);

	}
	printf("%30s: %f\n", "Average Block Size = ", mean);
	printf("%30s: %f\n", "Standard Deviation = ", sqrt(std_dev));

	/* report error code status */
	if(EXIT_STATUS == 1)
		printf("There were problems.\n");

	if(EXIT_STATUS == 0)
		printf("There were no problems.\n");

	/* clean things up */
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

	/* get the allocation object that corresponds to addr */
	struct allocation* a = is_in_hashtable(addr);

	/* if the object does not exist, then it has not been allocated and print out an error */
	if(a == NULL)
	{
		fprintf(stderr, "\nERROR: Memory not allocated\n\n %15s: %s\n %15s: %p\n\n",
			"at", where, "target memory", addr);
		EXIT_STATUS = 1;
		exit(0);
	}

    /* if the allocation object has already been freed, print out an error */
	if(a->active == false)
	{
		fprintf(stderr, "Error. Memory already freed at %s\n", where);
		EXIT_STATUS = 1;

		exit(0);
	}

	/* set the hash table entry to be inactive and then free the memory */
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

    /* print out an error message if the allocation is over 128MB */
	if(size >= 128000000){
		fprintf(stderr, "\nERROR: Allocation over 128MB\n\n %15s: %s\n %15s: %zu\n\n",
			"at", where, "size", size);
		EXIT_STATUS = 1;
		exit(0);
	}

	time_t now;
	time(&now);

	/* allocate the memory */
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

void leak_detection() {

	bool mem_leak_found = false;
	char* mem_leak_loc;

	int i;
	for(i = 0; i < ht->size; ++i)
	{
		if(ht->table[i] == NULL)
			continue;
		if (ht->table[i]->active==true ) {
			mem_leak_found = true;
			mem_leak_loc   = ht->table[i]->loc;
		}
	}

	if (mem_leak_found) {
		printf("%s: %p\n\n", "Error. Memory leak detected at ", mem_leak_loc);
		EXIT_STATUS = 1;
		exit(0);
	}

}

/* -----------init_hashtable()-----------
 * @params: none
 * @precondition: none
 * @postcondition: creates a new hashtable of size 2049 (arbitrary size, but also odd numbers)
 * -------------------------------------- */

void init_hashtable()
{
	ht = malloc(sizeof(struct hashtable));
	ht->table = calloc(2049, sizeof(struct allocation*));
	ht->size = 2049;
	ht->load = 0;

	/* run slug_memstats when the program exits */
	atexit(slug_memstats);
	atexit(leak_detection);
}

