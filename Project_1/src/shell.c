#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>

extern char **get_line(void);

int main(int argc, char *argv[]) {
   int i;
   char **args; 

   while(1) {

      printf("shell: ");
      args = get_line();
    
      if(!args) break;
      //printf(strcmp(args[0],"exit"));

      if(args[0] == NULL) {
         continue;
      }

      //exit?    
      if (strcmp(args[0],"exit") == 0) {
      	exit(0);
      }


      int numargs;
      
      for(i = 0; args[i] != NULL; i++) {
         //printf("Argument %d: %s\n", i, args[i]);
         numargs = i;
      }
      
      int backgrounding;

      //check for amperstand
      if (strcmp(args[numargs],"&") == 0) {
          backgrounding = 1;
          free(&args[numargs]);
          args[numargs] = NULL;
      };

      /*-----------------------------------------------------------
         execute a command with or without args, and no backgrounding
        ---------------------------------------------------------- */
      if (args[0] != NULL && strcmp(args[numargs],"&") != 0) {
         
         //create a child process
         pid_t child = fork();
         int status;

         //child process         
         if (child == 0) {
            int estatus = execvp(args[0],args);
            if (estatus == -1) {
                printf("error: %s not found\n",args[0]);
                exit(1);
            }
            else { 
             exit(0);
            }
         }
         //parent process
         else{
            pid_t wpid = wait(&status);
            printf("exit of %d was %d \n", (int)wpid, status);
         }
      } 

      /*------------------------------------------
         execute a command with multiple arguments
        ------------------------------------------*/

   }
   return 0;
}
