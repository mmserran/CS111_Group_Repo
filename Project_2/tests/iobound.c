#include <stdio.h>
#include <unistd.h>
#include <time.h>

/*only the most menial of tasks */
int checksum(time_t usertime)
{
	unsigned long total = 0;
	unsigned long i;
	char character;
	time_t starttime;
	time_t now; 
	time_t last_update_time;
	FILE* read_file; 
	FILE* write_file;
	time(&starttime);
	last_update_time = starttime;
	for (i = 0; i < 10000; ++i)
	{
		read_file = fopen("testread", "r");
		write_file = fopen("testwrite", "w");
		if(read_file == NULL)
		{
			printf("Failed to open file 'testread'.  Exiting\n");
			return 0;
		}
		character = fgetc(read_file);
		for( ; character != EOF; character = fgetc(read_file))
		{
			total += 1;
			fputc(character, write_file);
			time(&now);

			if ( last_update_time != now && (now - starttime) % 5 == 0 )
			{
				printf("IO bound process %d still running. %ld seconds in. Total is %ld\n", 
					getpid(), now - starttime, total);
				last_update_time = now;
			}

		}
		if(read_file != NULL)
		{
			fclose(read_file);
		}
		if(write_file != NULL)
		{
			fclose(write_file);
		}
	}
	return 0;
}


int main(int argc, char** argv)
{
	time_t usertime = 10;
	if (argv[1] != NULL) 
	{
		if( sscanf(argv[1], "%ld", &usertime) != 1)
		{
			printf("%s\n: invalid number.", argv[1]);
		}

	}
	printf("Running for %ld seconds...\n", usertime);
	checksum(usertime);
}