#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include "library.h"

//Name: David Ibilola
//Student Number: 22364006
//Student Email: david.ibilola@ucdconnect.ie


int main() {
    char str[250];
    int child_status;
    char** args;
    size_t size;
    char time[250];


    ShellPrompt(time, 250);

    if(signal(SIGINT, CtrlCHandler) == SIG_ERR)
    {
        perror("Unable to register signal handler");
        exit(EXIT_FAILURE);
    }

    while(!feof(stdin))
    {
        ShellInput(str, 250);

        args = malloc(sizeof(char*));
        size = Tokenizer(str, args);

        ShellProcess(child_status, args, size);
        free(args);
        ShellPrompt(time, 250);
    }


    return 0;
}


