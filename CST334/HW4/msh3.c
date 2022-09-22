#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * A simple shell
 */

#define MAX_BUF 160
#define MAX_TOKS 100

int main() {
    int ch;
    int counter = 0;
    int i;
    char *pos;
    char *tok;
    char s[MAX_BUF+2];   // 2 extra for the newline and ending '\0'
    static const char prompt[] = "msh> ";
    char *toks[MAX_TOKS];
    time_t rawtime;
    struct tm *timeinfo;

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

        // break input into tokens
        i = 0;
        char *rest = s;
        while ((tok = strtok_r(rest, " ", &rest)) != NULL && i < MAX_TOKS) {
            toks[i] = tok;
            i++;
            counter++;
        }
        if (i == MAX_TOKS) {
            printf("error: too many tokens\n");
            continue;
        }
        toks[i] = NULL;

        // if no tokens do nothing
        if (toks[0] == NULL) {
            continue;
        }

        // exit if command is 'exit'
        if (strcmp(toks[0], "exit") == 0) {
            break;
        }

        // print help info if command is 'help'
        if (strcmp(toks[0], "help") == 0) {
            printf("enter 'help', 'today', or 'exit' to quit\n");
            continue;
        }

        // print date if command is 'today'
        if (strcmp(toks[0], "today") == 0) {
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            printf("%02d/%02d/%4d\n", 1 + timeinfo->tm_mon, timeinfo->tm_mday, 1900 + timeinfo->tm_year);
            continue;
        }

        if(strcmp(toks[0], "cd") == 0) {
            printf("%d", counter);
            if(counter == 1)
                chdir(getenv("HOME"));
            else
                chdir(toks[1]);
            continue;
        }

        // otherwise fork a process to run the command
        int rc = fork();
        if (rc < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        }
        if (rc == 0) {
            // child process: run the command indicated by toks[0]
            execvp(toks[0], toks);  
            // if execvp returns than an error occurred
            printf("msh: %s: %s\n", toks[0], strerror(errno));
            exit(1);
        } else {
            // wait for command to finish running
            wait(NULL);
        }
    }
    exit(EXIT_SUCCESS);
}
