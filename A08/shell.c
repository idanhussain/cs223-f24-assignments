#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include <readline/readline.h>
#include <readline/history.h>

// Colors
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

// Length definitions
#define MAX_ARGS 64
#define MAX_PATH 2048
#define MAX_HOSTNAME 2048
#define MAX_PROMPT_SIZE 2048

char* create_prompt() {
	static char prompt[MAX_PROMPT_SIZE];  // Changed to static
	char hostname[MAX_HOSTNAME];
	char cwd[MAX_PATH];
	struct passwd *pw = getpwuid(geteuid());

	if (pw == NULL) {
		perror("getpwuid error");
		return "$ ";
	}

	// Current working directory
	if (getcwd(cwd, sizeof(cwd)) == NULL) {
		perror("getcwd error");
		strcpy(cwd, "???");
	}

	// Hostname
	if (gethostname(hostname, sizeof(hostname)) != 0) {
		perror("Hostname error");
		strcpy(hostname, "unknown");
	}

	hostname[MAX_HOSTNAME - 1] = '\0';
	cwd[MAX_PATH - 1] = '\0';

		int written = snprintf(prompt, MAX_PROMPT_SIZE,
				ANSI_COLOR_GREEN "%s@%s" ANSI_COLOR_RESET ":" 
				ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET "$ ",
				pw->pw_name, hostname, cwd);

	return prompt;
}

void parse_command(char *line, char **args) {
	char *token;
	int i = 0;
	token = strtok(line, " \t\n");

	while (token != NULL && i < MAX_ARGS - 1) {
		args[i++] = token;
		token = strtok(NULL, " \t\n");
	}
	args[i] = NULL;
}
int main() {
	char *line;
	char *args[MAX_ARGS];
	int status;

	printf("( ͡° ᴥ ͡°) \n");

	while (1) {
		char *prompt = create_prompt();
		line = readline(prompt);

		if (line[0] == '\0') {
			free(line);
			continue;
		}

		add_history(line);

		parse_command(line, args);

		// Exit
		if (strcmp(args[0], "exit") == 0) {
			free(line);
			printf("Hope you enjoyed my shell!\n");
			break;
		}

		// Execute command
		pid_t pid = fork();

		if (pid < 0) {
			perror("Fork failed");
			free(line);
			continue;
		}

		if (pid == 0) {  // Child
			execvp(args[0], args);
			perror("Command execution failed");
			exit(1);
		} else {  // Parent
			waitpid(pid, &status, 0);
			// Failed command error
			if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
				printf("Command exited with status %d\n", WEXITSTATUS(status));
			}
		}

		free(line);
	}

	return 0;
}
