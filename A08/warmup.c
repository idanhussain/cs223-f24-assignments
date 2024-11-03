#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	int status;

	pid_t pid;
	printf("[%d] A0\n", getpid());
	fflush(stdout);

	pid = fork();

	if(pid == 0) {
		printf("[%d] B1\n", getpid());
		fflush(stdout);
	} else {
		printf("[%d] B0\n", getpid());
		fflush(stdout);
		pid = wait(&status);

		pid = fork();

		if (pid == 0) {
			printf("[%d] C1\n", getpid());			
			fflush(stdout);
		} else {
			printf("[%d] C0\n", getpid());
			fflush(stdout);
		}
	}
	printf("[%d] Bye\n", getpid()); 

	return 0;
}
