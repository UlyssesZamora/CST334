#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

/*
 * A very simple shell that supports only commands 'exit', 'help', and 'today'.
 */

#define MAX_BUF 160
#define MAX_TOKS 100

int main() {
	int ch;
	char *pos;
	char s[MAX_BUF+2];   // 2 extra for the newline and ending '\0'
	static const char prompt[] = "msh> ";
	char *toks[MAX_TOKS];
    char *tokens;

	// 
	// YOUR CODE HERE   (add declarations as needed)
	//

	while (1) {
		// prompt for input if input from terminal
		if (isatty(fileno(stdin))) {
			printf(prompt);
		}

		// read input
		char *status = fgets(s, MAX_BUF+2, stdin);

		// exit if ^d entered
		if (status == NULL) {
			printf("\n");
			break;
		}

		// input is too long if last character is not newline 
		if ((pos = strchr(s, '\n')) == NULL) {
			printf("error: input too long\n");
			// clear the input buffer
			while ((ch = getchar()) != '\n' && ch != EOF) ;
			continue; 
		}

		// remove trailing newline
		*pos = '\0';

		//
		// YOUR CODE HERE
		//

        tokens = strtok(status, " ");
        if (tokens == NULL) {
            
        } else {
            while (tokens != NULL) {
                if(strcmp(tokens, "help") == 0) {
                    printf("%s", "enter Linux commands, or 'exit' to exit\n");
                    break;
                } else if (strcmp(tokens, "exit") == 0) {
                    exit(EXIT_SUCCESS);
                } else if (strcmp(tokens, "today") == 0) {
                    time_t currTime = time(0);
                    struct tm *info = localtime(&currTime);
                    printf("%02d/%02d/%d\n", info->tm_mon + 1, info->tm_mday, info->tm_year + 1900);
                } else {
                    printf("token: \'%s\'\n", tokens);
                }
                tokens = strtok (NULL, " ");
            }
        }
	}
	exit(EXIT_SUCCESS);
}
