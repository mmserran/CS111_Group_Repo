#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

extern char **get_line(void);
char* out_filename;
char* in_filename;

int i;
int background; 
int isCommand;
int shouldWait;

int execute(char** cmds){

  pid_t forkstatus = fork();
  if(forkstatus == -1){
    fprintf(stderr, "%s: %s\n", cmds[0], strerror(errno));
  }
  else if(forkstatus != 0){
    int execstatus;
    if(!background){
      wait(&execstatus);
      return execstatus;
    }
    else{
      waitpid(-1, &execstatus, WNOHANG);
      return execstatus;
    }
  }    
  else{
    if(out_filename != NULL){
      FILE* file;
      if ((file = fopen(out_filename, "w")) == NULL){
        fprintf(stderr, "%s: %s\n", cmds[0], strerror(errno));
        return;
      }
      int fd = fileno(file);
      dup2(fd, STDOUT_FILENO);
    }
    if(in_filename != NULL){
      FILE* file;
      if ((file = fopen(in_filename, "r")) == NULL){
        fprintf(stderr, "%s: %s\n", cmds[0], strerror(errno));
        return;
      }
      int fd = fileno(file);
      dup2(fd, STDIN_FILENO);

    }
    execvp(cmds[0], &cmds[0]);
    fprintf(stderr, "%s: %s\n", cmds[0], strerror(errno));
  }
}

int parse(char** args){
  background = 0;
  isCommand = 1;
  out_filename = NULL;
  in_filename = NULL;
  char* commands [64]; 
  int commandi = 0;
  for(i = 0; args[i] != NULL; i++) {
    if (strcmp("<", args[i]) == 0){
      in_filename = args[++i];
      isCommand = 0;
    }
    else if (strcmp(">", args[i]) == 0){
      out_filename = args[++i];
      isCommand = 0;
    }
    else if (strcmp("&", args[i]) == 0){
      background = 1;
      isCommand = 0;
    }
    else {
      if (isCommand) commands[commandi++] = args[i];
    }
  }
  commands[commandi++] = NULL;
  execute(commands);
}

int main(void) {
  char** args;
  char line[4096]; 
  while(1) {
    printf("%%: ");
    args = get_line();
    if(!args) break;
    if (strcmp(args[0], "exit") == 0) break;
    parse(args);
  }
}
