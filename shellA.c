#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE 40 /* 40 chars per line, per command, is enough. */

char command[200];
int numOfCmd = 0;
/** The setup() routine reads in the next command line string storing it in the input buffer.
The line is separated into distinct tokens using whitespace as delimiters.  Setup also 
modifies the args parameter so that it holds points to the null-terminated strings  which 
are the tokens in the most recent user command line as well as a NULL pointer, indicating the
end of the argument list, which comes after the string pointers that have been assigned to
args. ***/

void setup(char inputBuff[], char *args[],int *background)
{
    int length,  /* Num characters in the command line */
        i,       /* Index for inputBuff arrray          */
        j,       /* Where to place the next parameter into args[] */
        start;   /* Beginning of next command parameter */

    /* Read what the user enters */
    length = read(STDIN_FILENO, inputBuff, MAXLINE);  

    j = 0;
    start = -1;

    if (length == 0)
        exit(0);            /* Cntrl-d was entered, end of user command stream */

    if (length < 0){
        perror("error reading command");
	exit(-1);           /* Terminate with error code of -1 */
    }
	
    if (strncmp(inputBuff, "rr", 2) == 0){
            memset(inputBuff, 0, sizeof(inputBuff));
            strcpy(inputBuff, commandHistory[numOfCmd - 1]);
        if ((strncmp(inputBuff, "history", 7) == 0) || (strncmp(inputBuff, "h", 1) == 0)){
            int top;
            if (numberOfCommands < 5){
                top = numOfCmd;
            }
            else{
                top = 5;
            }

            int a = 0;
            while (a < top){
		ba = a + 1;
                printf("Command #%d: %s\n", ba, &(command + 40*a));
                a++;
            }
        }       
    }
    else if(strncmp(inputBuff, "r", 1) == 0){
        if(length < 4){ // For r0 through r9
            int commandNum = inputBuff[1] - '0';
            memset(inputBuff, 0, sizeof(inputBuff));
            strcpy(inputBuff, &(command + 40*commandNum));
        }
        if ((strncmp(inputBuff, "history", 7) == 0) || (strncmp(inputBuff, "h", 1) == 0)){
            int top;
            if (numberOfCommands < 5){
                top = numOfCmd;
            }
            else{
                top = 5;
            }

            int a = 0;
            while (a < top){
		ba = a + 1;
                printf("Command #%d: %s\n", ba, &(command + 40*a));
                a++;
            }
        }  
    }

    if (!((strncmp(inputBuff, "history", 7) == 0) || (strncmp(inputBuff, "h", 1) == 0))){

        // Add the Command to history
        memset((command[(numOfCmd % 5)*40]), 0, 40);
        strcpy(command[(numOfCmd % 5)*40]), inputBuff);
        ++numOfCmd;
    }
	
    /* Examine every character in the input buffer */
    for (i = 0; i < length; i++) {
 
        switch (inputBuff[i]){
        case ' ':
        case '\t' :          /* Argument separators */

            if(start != -1){
                args[j] = &inputBuff[start];    /* Set up pointer */
                j++;
            }

            inputBuff[i] = '\0'; /* Add a null char; make a C string */
            start = -1;
            break;

        case '\n':             /* Final char examined */
            if (start != -1){
                args[j] = &inputBuff[start];     
                j++;
            }

            inputBuff[i] = '\0';
            args[j] = NULL; /* No more arguments to this command */
            break;

        case '&':
            *background = 1;
            inputBuff[i] = '\0';
            break;
            
        default :             /* Some other character */
            if (start == -1)
                start = i;
	}
 
    }    
    args[j] = NULL; /* Just in case the input line was > 40 */
} 

int main(void)
{
    char inputBuff[MAXLINE]; /* Input buffer  to hold the command entered */
    char *args[MAXLINE/2+1];/* Command line arguments */
    int background;         /* Equals 1 if a command is followed by '&', else 0 */
    

    strcpy(command,"");


    while (1){            /* Program terminates normally inside setup */

	background = 0;
	pid_t pid;

	printf("CSE2431Sh$ ");  /* Shell prompt */
        fflush(0);

        setup(inputBuff, args, &background);       /* Get next command */
  
	 if ((strncmp(inputBuff, "history", 7) == 0) || (strncmp(inputBuff, "h", 1) == 0)){
            int top;
            if (numberOfCommands < 5){
                top = numOfCmd;
            }
            else{
                top = 5;
            }

            int a = 0;
            while (a < top){
		ba = a + 1;
                printf("Command #%d: %s\n", ba, &(command + 40*a));
                a++;
            }
        }  
	    
	    
	pid = fork();
	 
 	if (pid == 0) execvp(args[0], args);
	else if (background == 0) {
		waitpid(pid);
	}
	sleep(1);
    }
}
