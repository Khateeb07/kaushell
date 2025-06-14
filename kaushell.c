#include "kaushell.h"
static void perror_exit(char* s){
	perror(s);
	exit(1);
}
int main(int argc, char* argv[]){
	char** chargv;
	char inbuf[MAX_BUFFER];
	pid_t pid;
	printBanner();
	while(1){
		fputs("\033[1;31mkaushell$ \033[0;33m", stdout);	// Puts "kaushell$ " on the command line

// Stores input in 'inbuf' and checks for NULL
		if ((fgets(inbuf, MAX_BUFFER, stdin))==NULL)
			continue;
		inbuf[strlen(inbuf)-1]='\0';	// Storing '\0'(NULL) at length-1

// Maintaining history for each command in 'logFile.txt'
		if(inbuf[0]!='\0' && inbuf[0]!=' ' && inbuf[0]!='\n' && inbuf[0]!='\t') maintainLogs(inbuf);

// Handling 'logs' command...
		if(strcmp(inbuf, LOGS)==0) {
			displayLogs();
			continue;
		}

// Handling 'quit' command...
		if(strcmp(inbuf, QUIT_STRING)==0)
			exit(0);
// Forking is done and main process will be divided in 2 processes
		switch(pid=fork()) {
			case -1:
				printf("Something Went Wrong\n");
				break;
			case 0:
				executeCmd(inbuf);
				exit(1);
			default:
				wait(NULL);
				break;
		}
	}
	return 0;
}
void executeCmd(char* cmd){
	int child, count, i;
	int fds[2];
	char** chargv;
	pid_t pid;
//	int in_fd=0;
	char** pipelist;
	count=getArgv(cmd, "|", &pipelist);
	if(count<=0) {
		fprintf(stderr, "Failed to find any argument\n");
	} else {
		for(i=0; i<count; i++) {
			if(pipe(fds)==-1) {
				perror_exit("Failed to create pipes\n");
			} else if((child=fork())==-1) {
				perror_exit("Failed to create process to run command\n");
			} else if(child==0) {
				if(i<count-1 && dup2(fds[1], STDOUT_FILENO)==-1)
					perror_exit("Failed To Connect Pipeline\n");
				if(close(fds[0]) || close(fds[1]))
					perror_exit("Failed To close needed files\n");
				executeRedirect(pipelist[i], i==0, 0);
				exit(1);
			}
			if(dup2(fds[0], STDIN_FILENO)==-1)
				perror_exit("Failed to connect last component\n");
			if(close(fds[0]) || close(fds[1]))
				perror_exit("Failed To Final Close\n");
			wait(NULL);
		}
		executeRedirect(pipelist[i], i==0, 0);
		exit(1);
	}
	if(parseAndRedirectOut(cmd)==-1)
		perror("Failed to redirect Output\n");
	else if(parseAndRedirectIn(cmd)==-1)
		perror("Failed to redirect Input\n");
	else if(getArgv(cmd, "  \t", &chargv)<=0)
		perror("Failed To Parse Command\n");
	else {
		execvp(chargv[0], chargv);
		perror("Failed To Execute Command\n");
	}
}
int getArgv(const char* s, const char* delims, char*** argvp){
	int error, i, numtoken=0;
	const char* snew;
	char* t;
	if((s==NULL) || (delims==NULL) || (argvp==NULL))
		*argvp=NULL;
	snew=s+strspn(s, delims);
        if((t=malloc(strlen(snew)+1))==NULL)
		return -1;
	strcpy(t, snew);
	if(strtok(t, delims)!=NULL) {
		for(numtoken=1; strtok(NULL, delims)!=NULL; numtoken++);
	}
	if((*argvp=malloc((numtoken+1)*sizeof(char*)))==NULL) {
		free(t);
		return -1;
	}
	if(numtoken==0)
		free(t);
	else {
		strcpy(t, snew);
		**argvp=strtok(t, delims);
		for(i=1; i<numtoken; i++) {
			*((*argvp)+i)=strtok(NULL, delims);
		}
		*((*argvp)+numtoken)=NULL;
		return numtoken;
	}
}
int parseAndRedirectIn(char* cmd){
	int error, infd;
	char* infile;
	if((infile=strchr(cmd, '<'))==NULL)
		return 0;
	*infile='\0';
	infile=strtok(infile+1, " \t");
	if(infile==NULL)
		return 0;
	if((infd=open(infile, O_RDONLY))==-1)
		return -1;
	if(dup2(infd, STDIN_FILENO/*Terminal File No.*/)==-1) {
		error=errno;
		close(infd);
		errno=error;
		return -1;
	}
	return close(infd);
}
int parseAndRedirectOut(char* cmd){
	int error, outfd;
	char* outfile;
	if((outfile=strchr(cmd, '>'))==NULL)
		return 0;
	*outfile='\0';
	outfile=strtok(outfile+1, " \t");
	if(outfile==NULL)
		return 0;
	if((outfd=open(outfile, FFLAG, FMODE))==-1)
		return -1;
	if((dup2(outfd, STDOUT_FILENO))==-1) {
		error=errno;
		close(outfd);
		errno=error;
		return -1;
	}
	return close(outfd);
}
void executeRedirect(char* s, int in, int out) {
	char** chargv;
	char* pin;
	char* pout;
	if(in && ((pin=strchr(s, '<'))!=NULL) && out && ((pout=strchr(s, '>'))!=NULL) && (pin>pout)) {
		if(parseAndRedirectIn(s)==-1) {
			perror("Failed to redirect input\n");
			return ;
		}
		in=0;
	}
	if(out && (parseAndRedirectOut(s)==-1))
		perror("Failed to redirect output\n");
	else if(in && (parseAndRedirectIn(s)==-1))
		perror("Failed to redirect input\n");
	else if(getArgv(s, " \t", &chargv)<=0)
		fprintf(stderr,"Failed to parse command\n");
	else {
		execvp(chargv[0], chargv);
		perror("Failed to execute command\n");
	}
	exit(1);
}
void printBanner() {
	system("clear");
	printf("\033[1;36m");
	for(int i=0; i<80; i++)
		printf("=");
	printf("\n\n%*s%s%*s\n", 30, "", "Welcome to KAUSHELL", 30, "");
	printf("%*s%s%*s\n", 23, "", "Developed by Khateeb Aamir Usmani", 23, "");
//	printf("%*s%s%*s\n\n", 30, "", "kaushell version 1.0", 30, "");
	for(int i=0; i<80; i++)
		printf("=");
	printf("\nType 'quit' to quit the shell.\n\n");
}
void maintainLogs(char inbuf[]) {
	FILE* file=fopen("logFile.txt", "a+");
	int i=0;
	while(i<strlen(inbuf))
		fputc(inbuf[i++], file);
	fputc(10, file); // Adds new line to each command
	fclose(file);
}
void displayLogs() {
	FILE* file=fopen("logFile.txt", "r");
	int ch;
	while((ch=fgetc(file))!=EOF)
		fputc(ch, stdout);
	fclose(file);
}
