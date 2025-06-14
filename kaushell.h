#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#define FFLAG (O_WRONLY|O_CREAT|O_TRUNC)
#define FMODE (S_IRUSR|S_IWUSR)
#define MAX_BUFFER 256
#define LOGS "logs"
#define QUIT_STRING "quit"
int parseAndRedirectIn(char*);
int parseAndRedirectOut(char*);
void executeCmd(char*);
int getArgv(const char*, const char*, char***);
void executeRedirect(char*, int, int);
static void perror_exit(char*);
void printBanner();
void maintainLogs(char*);
void displayLogs();
