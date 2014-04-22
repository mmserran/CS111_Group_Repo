/*
   Author: James Shedel jshedel@ucsc.edu
   Assignment: CMPS111 Assignment 1, shell.c
   Date: 4/16/2014
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>

extern char **get_line(void);

void term_handler(int signal) {
   int status;
   waitpid(-1,&status,WNOHANG);   
   printf("\nHandler: child terminated with status %d\n",WEXITSTATUS(status));
}

int main(int argc, char *argv[]) {
   int i;
   char **args; 
   int bg;
   int out;
   char *out_file;
   int in;
   char *in_file;

   while(1) {

      printf("shell: ");
      args = get_line();
    
      if(!args) break;

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
      
      bg = 0;

      /*check for amperstand
        if it exists, remove it from the arguments
       */
      if (strcmp(args[numargs],"&") == 0) {
          bg = 1;
          free(args[numargs]);
          args[numargs] = NULL;
      }


      //if out > 0, then there is output redirection
      out = check_out(args,&out_file);
      if (out  < 0) {
         printf("syntax error in output redirection\n");
      }
   
      //if in > 0, then there is input redirection
      in = check_in(args,&in_file);
      if (in < 0) {
         printf("syntax error in input redirection\n");
      }     

      //execute the command
      exec_cmd(args,bg,out,out_file,in,in_file);      
   
     }
   return 0;
}

/*---------------------------------------------------------
* check_in:    check for output redirection
*
* @params:     args -> arguments array
*              out_file -> pointer to input file for freopen
*
* @return:     0 -> no output redirection
*              1 -> output redirection
*             -1 -> error parsing redirect
----------------------------------------------------------*/

int check_out(char **args, char **out_file) 
{
   int i;
   for (i = 0; args[i] != NULL; i++) {
      //check for >
      if (strcmp(args[i],">") == 0) {
         free(args[i]);
         args[i] == NULL;

         if (args[i+1] != NULL) {
            *out_file = args[i+1];
         }
         else {
            return -1;
         }

         adj_args(args,i);
         return 1;
      }
   }
   //no output
   return 0;
}

/*---------------------------------------------------------
* check_in:    check for input redirection
*
* @params:     args -> arguments array
*              in_file -> pointer to input file for freopen
*
* @return:     0 -> no input redirection
*              1 -> input redirection
*             -1 -> error parsing redirect
----------------------------------------------------------*/

int check_in(char **args, char **in_file) 
{
   int i;
   for (i = 0; args[i] != NULL; i++) {
      //check for <
      if (strcmp(args[i],"<") == 0) {
         free(args[i]);
         args[i] == NULL;
         
         if (args[i+1] != NULL) {
            *in_file = args[i+1];
         }
         else {
            return -1;
         }

         adj_args(args,i);
         return 1;
      }
   }
   //no input
   return 0;
}         

/*----------------------------------------------------------
*
* adj_args:      adjust arguments array
*           
*  @params:    args -> arguments array
*              i -> spot at which > or < is found
*
*  @return:    takes any arguments of the command and shifts
*              them so that they are correctly passed to the
*              command after redirect args have been taken out
*
-----------------------------------------------------------*/


int adj_args(char **args, int i) 
{
   int k;
   for (k = i; args[k-1] != NULL; k++) {
      args[k] = args[k+2];
   }
}


/*-----------------------------------------------------------
*   
* exec_cmd:      execute a command
*         
*  @params:  args -> arguments to command 
*            bg -> backgrounding (1=yes/0=no)
*            out -> redirect output (1=yes/0=no)
*            out_file -> file to redirect ouput to
*            in -> redirect input (1=y/0=n)
*            in_file -> redirect input from file 
*
*  @return:   none
*
------------------------------------------------------------- */

int exec_cmd(char **args, int bg, int out, char* out_file, int in, char* in_file) 
{
      if (args[0] != NULL) {
          
         //create a child process
         pid_t child = fork();
         int status;

         //child process         
         if (child == 0) {
            if (out) {
               freopen(out_file, "w+", stdout);
            }
            if (in) {
               freopen(in_file,"r",stdin);
            }
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
            if (bg) {
               //set up the signal handler
               sigset(SIGCHLD,term_handler);
            }
            if (!bg) {
               //sigset(SIGCHLD,SIG_DFL);
               pid_t wpid = wait(&status);
               //printf("exit of %d was %d \n", (int)wpid, status);
            }
         }
      } 
}
