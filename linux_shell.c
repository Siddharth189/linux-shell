// including all necessary headers

#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h> //for using function related to directory listings



int setpgid(pid_t pid, pid_t pgid);
struct dirent *readdir(DIR *dirp);
int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);
int errno;



#define MAX_INPUT_SIZE 1024 // maximum  input size
#define MAX_TOKEN_SIZE 64 // maximum token size of 1 token
#define MAX_NUM_TOKENS 64 // maximum number of tokens allowed


//print tokens
void printTokens(char ** tokens){
        int size = sizeof(tokens)/sizeof(tokens[0]);
        for(int i=0; i<size; i++){
                printf("%s", tokens[i]);
        }
        printf("\n");
}


//splits the string by space and returns the array of tokens
//this function will separate the line string into different words/tokens
//input : "sleep 5"
//output : ["sleep" ,"5"]
char **tokenize(char *input)
{
        //allocate memory for tokens array & for each token
        char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
        char *tok = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
        int p;
        int token_Index = 0;//for each token
        int token_No = 0; //for index of tokens array

        //it goes over each character of input
        for(p =0; p < strlen(input); p++)
        {
                //read character by character
                char readChar = input[p];

                //check if it is a white space characters
                //then insert that token into tokens array on token_No position
                if (readChar == ' ' || readChar == '\n' || readChar == '\t'){

                        tok[token_Index] = '\0';
                        if (token_Index != 0)
                        {
                                // printf("token: %s" ,tok );
                                // printf("\n");
                                //allocate memory for token in tokens array
                                tokens[token_No] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
                                strcpy(tokens[token_No++], tok); //copy that token in tokens array on token_No pos and do ++
                                token_Index = 0; //reset it for next token
                        }
                }
                else
                {
                        //not a white space characters,then add it to token
                        tok[token_Index++] = readChar;
                }
        }

        //finally free memory and return tokens array
        free(tok);
        tokens[token_No] = NULL ;
        return tokens;
}



int main(int argc, char* argv[])
 {

        char  input_line[MAX_INPUT_SIZE]; //store the input
        char  **tokens; //stores the tokens
        int i;

        printf("\n\n******** Welcome to my shell ********\n\n");

        int back=0;
        while(2)
        {
                // erase the given memory area with zero bytes(\0)
                bzero(input_line, sizeof(input_line));
                printf("siddharth@shell> ");

                //take input from user
                //will read all characters until you reach \n (or EOF)
                scanf("%[^\n]", input_line);
                getchar(); //it will wait for the user to enter a new line character

                //user has hit enter only without typing anything
                if(strcmp(input_line,"\0") ==0 )
                        continue;


                //input string terminated with new line
                input_line[strlen(input_line)] = '\n';



                //get tokens array
                tokens = tokenize(input_line);

                printTokens;


                //handling different commands
                if(strcmp(tokens[0],"exit") == 0 )  //to handle exit command
                {
                        printf("***************Exiting****************\n");
                        exit(0);

                }

                if(strcmp(tokens[0],"cd") ==0 )  // to handle cd command
                {
                        // chdir(const char *path) : changes the current working directory of the calling process to the directory specified in path.
                        //On success, zero is returned.  On error, -1 is returned,
                        char str[1000];
                        if(chdir(tokens[1])== 0)
                                printf("changed dir. to : %s\n",getcwd(str,1000));
                        continue;
                }

                if(strcmp(tokens[0],"pwd") ==0 ) // to handle pwd command
                {
                        //char *getcwd(char *buf, size_t size) : The getcwd() function copies an absolute pathname of the current working directory to the array pointed to by buf, which is of length size.
                        //On success, these functions return a pointer to a string containing the pathname of the current working directory
                        //On failure, these functions return NULL
                         char str[1000];
                        printf("current dir. : %s\n",getcwd(str,1000));
                        continue;
                }


                if(strcmp(tokens[0],"ls") ==0 )  // to handle ls command
                {  {
                           //he opendir() system call function is used to open a directory and to return a pointer on this directory
                           DIR *dir = opendir(".");
                           struct dirent *dp;

                           // read each entry in directory
                           /*If successful, readdir() returns a pointer to a dirent structure describing the next directory entry in the directory stream.
                            When readdir() reaches the end of the directory stream, it returns a NULL pointer and and sets errno */
                           while ((dp = readdir(dir)) != NULL)
                                   printf("%s  ", dp->d_name);

                          // Close directory
                           closedir(dir);
                          printf("\n");
                           continue;
                }}


                //create a new child process in order to execute different commands
                pid_t p_id = fork();


                for( i=0; tokens[i+1] !=NULL; i++ );

                back =0;

                if( strcmp(tokens[i],"&" ) == 0 )
                {
                        back =1;
                        tokens[i]=NULL;
                }

                //forking failed
                if (p_id == -1)
                 {
                        printf("\nFailed");
                        return -1;
                }
                 else if (p_id == 0)
                  {
                        //child process
                        //take executable name as first argument
                        //it will search for the executable in the linux system , if it finds it then it will reintialize the memory image of child process with code of this executable
                        if (execvp(tokens[0],tokens) < 0 )
                        {       //unable to find executable
                                printf("\nUnable to execute given command..\n");
                        }
                        exit(0);
                }
                else
                 {
                        //parent process
                        //parent will wait for the child process to exit
                        if(back==0)
                                waitpid(p_id,NULL,0);

                }


                //parent will clear all the memory used by child
                for(i=0;tokens[i] != NULL; i++ )
                {
                        free(tokens[i]);
                }
                free(tokens);

        }
        return 0;
}
