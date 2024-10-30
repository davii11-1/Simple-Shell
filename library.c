#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include "library.h"

void ShellInput(char * str, int size)
{
    fgets(str, size, stdin);
    isCtrlDEntered();
    size_t len = strlen(str);
    str[len-1] = '\0';
}

int Tokenizer(char * str, char ** argv)
{
    int i = 0;
    char * token = strtok(str, " ");

    while(token != NULL)
    {
        argv[i++] = token;
        argv = realloc(argv,(i+1) * sizeof(char*));
        token = strtok(NULL, " ");
    }
    argv[i] = NULL;
    return i;
}

void isCtrlDEntered()
{
    if(feof(stdin))
    {
        printf("\n");
        fflush(stdout);
        exit(EXIT_SUCCESS);
    }
}

void ShellProcess(int status, char** argv, size_t size)
{
    int conditionForFeature3=0;
    int child_id;
    char * str = argv[1];
    int conditionForFeature5 = 0;
    char * filename;
    int f;

    if(strcmp(argv[0], "cd") == 0)
    {
        conditionForFeature3 = 1;
    }


    for(int j = 0; j < size; j++)
    {

        if(strcmp(argv[j], ">") == 0)
        {
            if(argv[j+1] == NULL)
            {
                perror("Invalid redirect attempt");
                fflush(stdout);
                return;
            }

            filename = argv[j+1];
            conditionForFeature5 = 1;
            argv[j] = NULL;
            argv = realloc(argv, (j+1) * sizeof(char*));
            break;
        }
    }

    child_id = fork();
    if(child_id == -1)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    if(child_id == 0) {
        if(conditionForFeature3 == 0) {
            if(conditionForFeature5 == 1) {
                f = open(filename,  O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if(f == -1)
                {
                    perror("ERROR");
                    exit(EXIT_FAILURE);
                }

                if(dup2(f,STDOUT_FILENO) == -1)
                {
                    perror("ERROR");
                    exit(EXIT_FAILURE);
                }

                close(f);
                
                execvp(argv[0], argv);
                perror("Unknown command");
                fflush(stdout);
                return;
            }
            else {
                execvp(argv[0], argv);
                perror("Unknown command");
                fflush(stdout);
                return;
            }
        }
        else {
            if(argv[1] == NULL)
            {
                str = getenv("HOME");
            }

            if(chdir(str) != 0)
            {
                perror("ERROR");
                fflush(stdout);
                exit(EXIT_FAILURE);
            }
            return;
        }
    }
    else {
        signal(SIGINT, SIG_IGN);
        wait(&status);
        signal(SIGINT, SIG_DFL);
    }
}

void CurrentTime(char * strtime, int size)
{
    time_t sec_time;
    struct tm* local_time;

    sec_time = time(NULL);
    local_time = localtime(&sec_time);
    strftime(strtime, sizeof(char) * size, "[%d/%m %H:%M]# ", local_time);
}

void ShellPrompt(char * str, int size)
{
    CurrentTime(str, size);
    printf("%s",str);
    fflush(stdout);
}

void CtrlCHandler(int signo)
{
    char str[250];
    if(signo == SIGINT)
    {
        printf("\n");
        ShellPrompt(str, 250);
        printf("^C\n");
        ShellPrompt(str, 250);
        printf("\n");
        exit(EXIT_SUCCESS);
    }
}