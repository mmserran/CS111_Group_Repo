
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
};

struct hashtable* ht = NULL;

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

void insert_into_hashtable(void* loc, char* where, time_t when, size_t size)
{
	assert(ht->size != ht->load + 5);
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
	++ht->load;
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

//returns false if element wasn't found
struct allocation* delete_from_hashtable(void* loc, char* where)
{
	size_t pos = (unsigned long) loc % ht->size;
	while(ht->table[pos] != NULL)
	{
		if(ht->table[pos]->loc == loc)
		{
			//free(ht->table[pos]);
			ht->table[pos]->active = false;
			ht->table[pos]->where = where;
			//--ht->load;
			return ht->table[pos];
		}
	}
	return NULL;
}



void slug_free ( void *addr, char *where )
{
	if(ht == NULL)
	{ 
		fprintf(stderr, "%s:Error. Invalid Free, memory not allocated.\n", where);
		exit(1);
	}
	struct allocation* a = delete_from_hashtable(addr, where);

	if(a == NULL)
	{
		fprintf(stderr, "%s:Error. Invalid Free, memory not allocated.\n", where);
		exit(1);
	}

	if(a->active == false)
	{
		fprintf(stderr, "Error. Memory already freed at %s\n", where);
		exit(1);
	}

}

void slug_memstats ( void )
{
	int i;
	struct tm * timeinfo;
	for(i = 0; i < ht->size; ++i)
	{
		if(ht->table[i] == NULL)
			continue;

		timeinfo = localtime(&ht->table[i]->when);
		//for debugging only
		printf("At entry %d ...  ", i);

		printf("%p: size %zu: allocation time %s: ",
			 ht->table[i]->loc, ht->table[i]->size, asctime(timeinfo)); //STOPPED HERE

		if(ht->table[i]->active == true)
			printf("Allocated at %s\n", ht->table[i]->where);
		else
			printf("Freed at %s\n", ht->table[i]->where);
	}
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
}




/*
//only for testing purposes
int main()
{
	/*
	int i = 4;
	int b = 24;
	insert_into_hashtable(&i);
	printf("%d\n", is_in_hashtable(&i));
	printf("%d\n", is_in_hashtable(&b));
	printf("%d\n", delete_from_hashtable(&i));
	printf("%d\n", is_in_hashtable(&i));
		slug_malloc(8, FILE_POS);
	slug_memstats();
	destroy_hashtable();
	return 0;
}
*/