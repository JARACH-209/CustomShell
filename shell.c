#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h> 
#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 512
#define MAX_NUM_TOKENS 512
const char* bincommands[] = {"exit","cd","pwd"};
static char* cwd(){
	static char cw[32];
	getcwd(cw,32);
	return cw;
}
void shello(){
	printf("\033[H\033[J");
	printf("\n\tWelcome to Shell\t\n\n");
	char* s = cwd();
	printf("\nWorking Directory : %s\n",s);
}
int execute(char** args){	
  	pid_t pid, wpid;
  	int status;
  	pid = fork();
  	if (pid == 0) {											// Child process
    	if (execvp(args[0], args) == -1) {
    	 perror("Could not run\n");
    	}
    	exit(0);
  	}		 
  	else if (pid < 0) {										// Error in forking
    	perror("Child Could not be created/forked");
  	} 
  	else {													// Parent process
    	do {
      	wpid = waitpid(pid, &status, WUNTRACED);
    	} while (!WIFEXITED(status) && !WIFSIGNALED(status));
  	}
 	return 1;
}
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;
  for(i =0; i < strlen(line); i++){
    char readChar = line[i];
    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}
int main(){
	shello();
	int i=0;
	while(1){
		char* line;
		char** args;
		int flag=0,ownarg=0;
		printf("root@Jarach_2.0.9:> ");
		scanf("%[^\n]",line);
		getchar();
		line[strlen(line)] = '\n';
		args = tokenize(line);
    for (i = 0; i < 3; i++){
    	if (strcmp(args[0], bincommands[i]) == 0) { 
            ownarg = i + 1; 
            break; 
        } 
    }
    switch (ownarg) { 
    case 1: 
        printf("\nShell Exit\n"); 
        exit(0); 
        return 0;
    case 2: {
	++flag;
        int x = chdir(args[1]);
	if(x==-1) printf("Invalid Directory or Directory not found\n");
        break;}
    case 3: 
        printf("Dir: %s\n",cwd());
        ++flag;
        break; 
    default:{ 
	flag = execute(args);        
	break;
	} 
    } 
    if(flag==1)
		continue;
	else {
    	printf("\n...ExitShell...\n");
		return 0;	
	}
	}
	return 0;
}
