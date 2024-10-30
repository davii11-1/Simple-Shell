#ifndef UNIXA2_LIBRARY_H
#define UNIXA2_LIBRARY_H

#endif //UNIXA2_LIBRARY_H

void ShellInput(char * str, int size);
int Tokenizer(char * str, char **argv);
void isCtrlDEntered();
void ShellProcess(int status, char** argv, size_t size);
void CurrentTime(char * strtime, int size);
void ShellPrompt(char * str, int size);
void CtrlCHandler(int signo);
