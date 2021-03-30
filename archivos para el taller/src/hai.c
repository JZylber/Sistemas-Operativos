#include <signal.h> /* constantes como SIGINT*/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int supCounter = 0;

void yaVaHandler(int sig){
	printf("ya va!\n");
	fflush(stdout);
	supCounter++;
	return;
}

void childAnswer(int sig){
	wait(EXIT_SUCCESS);
	exit(EXIT_SUCCESS);
	return;
}

int main(int argc, char* argv[]) {
  	pid_t child = fork();
	if(child == 0){
		signal(SIGURG,yaVaHandler);
		while(supCounter<5){
		}
		pid_t parent_id = getppid();
		kill(parent_id,SIGINT);
		execvp(argv[1],argv+1);
	}
	else{
		signal(SIGINT,childAnswer);
		for(int i = 0;i < 5; i++){
			sleep(1);
			printf("sup!\n");
			fflush(stdout);
			kill(child,SIGURG);
		}
		while(true){
			
		}
	}
	return 0;
}
