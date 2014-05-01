#include <stdio.h>
#include <unistd.h>
#include <time.h>

/*only the most menial of tasks */
int increment(time_t usertime)
{
	unsigned long total = 0;

	time_t starttime;
	time_t now; 
	time_t last_update_time;

	time(&now);
	for(time(&starttime), last_update_time = starttime; total < 2000000000; time(&now) )
	{
		total += 1;  
		if ( last_update_time != now && (now - starttime) % 5 == 0 )
		{
			printf("CPU bound process %d still running. %ld seconds in. Total is %ld\n", 
				getpid(), now - starttime, total);
			last_update_time = now;
		}
	}
	printf("Process %d has finished in %ld seconds.\n", getpid(), now - starttime);
	return total;  
}


int main(int argc, char** argv)
{
	time_t usertime = 10;
	if (argv[1] != NULL) 
	{
		if( sscanf(argv[1], "%ld", &usertime) != 1)
		{
			printf("invalid number.\n");
		}

	}
	increment(usertime);
}