#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// max length input string
#define MAXSTR 25

// convert input binary string to a number


int power(int base, int timesToMultiply) {
    int num = 1;
    int i = 0;
    if(timesToMultiply == 0) {
        return num;
    }

    for(i = 0; i < timesToMultiply; i++) {
        num *= base;
    }
    return num;
}

int main() {

   // user input string
    char s[MAXSTR+3];
    int decimalValue = 0;
    int counter = 0;
    int i = 0;

   // prompt for input if input from terminal
   if (isatty(fileno(stdin))) {
       printf("> ");
   }

   // read input string; at most MAXSTR+1 chars accepted
   // Note: this is tricky. If we accept only MAXSTR chars,
   // we can't see if user entered more chars and they are
   // being dropped by fgets.
   fgets(s, MAXSTR+3, stdin);

   // check input length; n does not include final carriage return
   int n = strlen(s)-1;
   if (n > MAXSTR) {
     printf("input cannot be more than %d characters\n", MAXSTR);
     exit(1);
   }

   // convert s from a string in binary, to an int, and output

   for(i = 0; i < n; i++) {
        if((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= '2' && s[i] <= '9')) {
            printf("%s\n", "input must contain only zeros and ones");
            exit(1);
        }
   }

   for(i = n; i >= 0; i--) {
        int base = 2;
        if(s[i] == '1') {
            int tempValue = power(base, counter - 1);
            decimalValue += tempValue;
        }
        counter++;
   }

   printf("%d\n", decimalValue);
}

