//slugmem.h
#include <stddef.h>


void *slug_malloc ( size_t size, char *WHERE );

/*
	This function will allocate memory by calling malloc(). It returns the address of allocated 
	memory. In addition, it records the address, length, current timestamp, and location of the call
	in an internal data structure. If size is zero, this is not an error but should be reported on 
	stderr as an unusual operation. If the input is excessively large (more than 128 MiB) then this
	function should report an error in stderr and terminate the program. The parameter WHERE is 
	a string constant that records the filename and line number of the caller.
*/

void slug_free ( void *addr, char *WHERE );

/*
	This function first checks that the addr is the start of a valid memory region that is currently 
	allocated by looking through the internal data structures. If not, an errors is shown and the 
	program terminated. If it is valid then free() should be called and the internal data structures 
	updated to indicate that the address is no longer actively allocated.
*/

void slug_memstats ( void );

/*
	This function traverses the internal data structures kept by slug_malloc() and slug_free() 
	and prints out information about all current allocations. Each allocation report should include 
	the size of the allocation, a timestamp for when the allocation took place, the actual address of
	the allocation, and file and line number in the test program where the allocation happened. 
	In addition, a summary of all allocations should be reported that includes the total number of 
	allocations done, the number of current active allocations, the total amount of memory 
	currently allocated, and the mean and standard deviation of sizes that have been allocated.

*/



//#define malloc(s) slug_malloc((s))

//To get the file and line number you might find the following macros helpful:




#define FUNCTIONIZE(a,b) a(b)
#define STRINGIZE(a) #a
#define INT2STRING(i) FUNCTIONIZE(STRINGIZE,i)
#define FILE_POS __FILE__ ":" INT2STRING(__LINE__)

//You will need to add an argument to slug_malloc() and slug_free() to pass in FILE_POS.

#define malloc(s) slug_malloc((s), FILE_POS)

#define free(s) slug_free((s), FILE_POS)



