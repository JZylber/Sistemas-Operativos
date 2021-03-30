#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char* argv[]) {
	int status;
	pid_t child;

	if (argc <= 1) {
		fprintf(stderr, "Uso: %s comando [argumentos ...]\n", argv[0]);
		exit(1);
	}

	/* Fork en dos procesos */
	child = fork();
	if (child == -1) { perror("ERROR fork"); return 1; }
	if (child == 0) {
		long traceResult = ptrace(PTRACE_TRACEME,0,NULL,NULL);
		if(traceResult == -1){
			perror("ERROR traceMe"); exit(1);
		}
		/* S'olo se ejecuta en el Hijo */
		execvp(argv[1], argv+1);
		/* Si vuelve de exec() hubo un error */
		perror("ERROR child exec(...)"); exit(1);
	} else {
		/* S'olo se ejecuta en el Padre */
		while(1) {
			if (wait(&status) < 0) { perror("waitpid"); break; }
			if (WIFEXITED(status)) break;
			int sysno = ptrace(PTRACE_PEEKUSER, child, 8*ORIG_RAX, NULL);
			if(sysno == 62){
				ptrace(PTRACE_KILL,child,NULL,NULL);
				printf("El proceso %d ha intentado envı́ar la señal SIGKILL y por eso ha sido detenido.\n",child);
				exit(EXIT_SUCCESS);
			}
			ptrace(PTRACE_SYSCALL,child,NULL,NULL);
			 /* Proceso terminado */
		}
	}
	return 0;
}
