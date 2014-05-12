
#include "slugmem.h"

struct allocation
{
	void* loc;
};

struct hashtable
{
	size_t size;
	size_t load;
	struct allocation** table;
};

struct hashtable* ht;

void init_hashtable()
{
	ht = malloc(sizeof(struct hashtable));
	ht->table = calloc(2048, sizeof(struct allocation*));
	ht->size = 2048;
	ht->load = 0;
}

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

void insert_into_hashtable(void* loc)
{
	assert(ht->size != ht->load + 5);
	size_t pos = (unsigned long) loc % ht->size;
	while(ht->table[pos] != NULL)
	{
		pos = (pos + 1) % ht->size;
	}
	struct allocation* a = malloc(sizeof (struct allocation));
	a->loc = loc;
	ht->table[pos] = a;
	++ht->load;
}

bool is_in_hashtable(void* loc)
{
	size_t pos = (unsigned long) loc % ht->size;
	while(ht->table[pos] != NULL)
	{
		if (ht->table[pos]->loc == loc) return true;

		pos = (pos + 1) % ht->size;

	}
	return false;
}

//returns false if element wasn't found
bool delete_from_hashtable(void* loc)
{
	size_t pos = (unsigned long) loc % ht->size;
	while(ht->table[pos] != NULL)
	{
		if(ht->table[pos]->loc == loc)
		{
			free(ht->table[pos]);
			ht->table[pos] = NULL;
			--ht->load;
			return true;
		}
	}
	return false;
}


void* slug_malloc(size_t size, char* where)
{
	return NULL;
}


//only for testing purposes
int main()
{
	init_hashtable();
	int i = 4;
	int b = 24;
	insert_into_hashtable(&i);
	printf("%d\n", is_in_hashtable(&i));
	printf("%d\n", is_in_hashtable(&b));
	printf("%d\n", delete_from_hashtable(&i));
	printf("%d\n", is_in_hashtable(&i));

	return 0;
}