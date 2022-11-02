#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>  
#include <sys/types.h>
// C:\msys64\mingw64\bin (for resetting path)

#ifndef MAX_BUF
#define MAX_BUF 200
#define MAX_LIMIT 1000
#endif

#define FALSE 0 ... #define TRUE !(FALSE)

// char * getStr(char *str[]) {
//         int length = strlen(str[0]);
//         char *str1 = malloc(sizeof(char) * 1000);
//         char *start = str[0];

//         for (int i = 0; i < length; i++) {
//                 str1[i] = *(start + i);
//         }

//         return str1;
// }
sig_atomic_t signaled = 0;

void my_handler (int param)
{
  signaled = 1;
}

void strFetch(char *input, char output[]) {
        int length = strlen(input);
        for (int i = 0; i < length; i++) {
                output[i] = *(input + i);
        }

        //Removing trailing \n from char[] 
        output[strcspn(output, "\n")] = 0;
}

void getCurrentDir(char *result) {
        char path[MAX_BUF];
        char delim[] = "\\";
        getcwd(path, MAX_BUF);

        //Print the entire path
        //printf("Current working directory: %s\n", path);

        //First splice of the path
        char *ptr = strtok(path, delim);

        //str[] stores the spliced sections of the path
        int size = 1;
        char *str[size];

        //Tracker is for last index of str array
        int tracker = 0;

        while(ptr != NULL) {
                //  printf("'%s'\n", ptr);

                 //Splicing the path and adding it to the str array
                 ptr = (char *) strtok(NULL, delim);

                 //Adding the spliced section to the end of the str array
                 str[size] = ptr;

                 //Stop incrementing size and tracker if splicing is completed
                 if (ptr == NULL) {
                        break;
                 }

                 size += 1;
                 tracker += 1;
        }

        int length = strlen(str[tracker]);

        for (int i = 0; i < length; i++) {
                result[i] = *(str[tracker] + i);
        }
}

int splitInput(char *input, char *str1[]) {
        //Parsing the input
        char delim1[] = " ";
        char *ptr1 = strtok(input, delim1);

        //str1 will be an array of pointers storing each spliced section
        int size1 = 1;
        // char *str1[size1];
        int tracker1 = 0;

        str1[0] = ptr1;

        while(ptr1 != NULL) {
                ptr1 = (char *) strtok(NULL, delim1);
                str1[size1] = ptr1;
                if (ptr1 == NULL) {
                        break;
                }

                size1 += 1;
                tracker1 += 1;
        }

        return size1;
}

void readFile(FILE *file, char *fileName) {
        char directory[100] = {0};
        strFetch(fileName, directory);
        // printf("%s", directory);
        file = fopen(directory, "r");
        if (!file) {
                // printf("worked");
                printf("Error \n");
        }
        char ch;
        while ((ch=fgetc(file)) != EOF) {
                printf("%c", ch);
        }
        printf("\n");
        fclose(file);
}

void readFile2(FILE *file, char *fileName) {
        char directory[100] = {0};
        strFetch(fileName, directory);
        // printf("%s", directory);
        file = fopen(directory, "r");
        if (!file) {
                // printf("worked");
                printf("Error \n");
        }
        char ch;
        while ((ch=fgetc(file)) != EOF) {
                
        }
        printf("\n");
        fclose(file);
}

int writeFile(FILE *file, char *fileName) {
        char directory[100] = {0};
        strFetch(fileName, directory);
        // printf("%s", directory);
        file = fopen(directory, "w+");

        if (!file) {
                printf("Sorry. The file you are trying to open doesn't exist . . .\n");
                return 0;
        }
        
        char input[100];
        char ch;
        int a = 0;

        while (a == 0) {
                void (*handler)(int);
                handler = signal (SIGINT, my_handler);
                // raise(SIGINT);
                // printf ("signaled is %d.\n", signaled);

                // fgets(input, 100, stdin);
                gets(input);
                if (signaled == 1) {
                        signaled = 0;
                        break;
                }

                fprintf(file, "%s\n", input);
                input[0] = '\0';
        }

        printf("\n");
        fclose(file);
        return 1;
}

int writeFile1(FILE *file, char *fileName) {
        char directory[100] = {0};
        strFetch(fileName, directory);
        // printf("%s", directory);
        file = fopen(directory, "a");

        if (!file) {
                printf("Sorry. The file you are trying to open doesn't exist . . .\n");
                return 0;
        }
        
        char input[100];
        char ch;
        int a = 0;

        // while ((ch=fgetc(file)) != EOF) {
        //         printf("%c", ch);
        // }

        while (a == 0) {
                void (*handler)(int);
                handler = signal (SIGINT, my_handler);
                // raise(SIGINT);
                // printf ("signaled is %d.\n", signaled);

                // fgets(input, 100, stdin);
                gets(input);
                if (signaled == 1) {
                        signaled = 0;
                        break;
                }

                fprintf(file, "%s\n", input);
                input[0] = '\0';
        }

        printf("\n");
        fclose(file);
        return 1;
}

int main (int argc, char *argv[])
{
        int a = 0;

        while (a == 0) {
                //Prompt
                char curDir[100] = {0};
                getCurrentDir(curDir);
                printf("[nyush %s]$ ", curDir);

                //Input
                char input[100];
                char *str1[1];
                fgets(input, 100, stdin);

                //Parsing the input
                int size1 = splitInput(input, str1);

                //Empty line
                if (size1 == 0) {
                        continue;
                }

                //Testing execvp()
                pid_t pid, wpid;
                int status;

                pid = fork();
                if (pid == 0) {
                        // Child process
                        if (execvp(str1[0], str1[1]) == -1) {
                                perror("lsh");
                        }
                        exit(EXIT_FAILURE);
                }
                else if (pid < 0) {
                        // Error forking
                        perror("lsh");
                }
                else {
                        // Parent process
                        wait();
                        continue;
                }

                //Comparison string
                // char *cmpStr = "cd";
                // char *cmpStr1 = "jobs";
                // char *cmpStr2 = "cat";

                //Built-in commands
                if (str1[0] != NULL) {

                        //Absolute path (1 arg)
                        char directory[100] = {0};
                        strFetch(str1[0], directory);
                        char firstDirChar = directory[0];
                        char absStr = '/';

                        //Absolute path first arg (>= 1 arg)
                        if (firstDirChar == absStr) {
                                // printf("triggered");
                                chdir(directory);
                                continue;
                        }

                        //< Redirection operator
                        if ((strcmp(str1[1], "<")  == 0) && (size1 % 2 != 0) && (size1 > 2)) {
                                if (strcmp(str1[0], "cat") == 0) {
                                        FILE* fptr;
                                        readFile(fptr, str1[2]);
                                }
                        }

                        //cd command (2 args)
                        if (strcmp(str1[0], "cd") == 0 && size1 == 2) {
                                //printf("Correct argument for cd \n");
                                char directory[100] = {0};
                                strFetch(str1[1], directory);
                                char firstDirChar = *directory;
                                char absStr = '/';

                                //printf("%c", firstDirChar);
                                printf("%s \n", directory);
                                //printf("%i", strcmp(&firstDirChar, absStr));

                                //Absolute path
                                if (firstDirChar == absStr) {
                                        // printf("%i", chdir(str1[1]));
                                }

                                char cwd[PATH_MAX];

                                // printf("%s \n", getcwd(cwd, sizeof(cwd)));
                                if (chdir(directory) != 0) {
                                        fprintf( stderr, "Error: invalid directory\n");
                                }
                                // printf("%s \n", getcwd(cwd, sizeof(cwd)));
                        }

                        //cd command error (not 2 args)
                        else if (strcmp(str1[0], "cd") == 0 && size1 != 2) {
                                fprintf( stderr, "Error: invalid command\n");
                        }

                        //Unfinished jobs command implementation
                        else if (strcmp(str1[0], "jobs") == 0 && size1 == 1) {
                                
                        }

                        //cat command 
                        else if (strcmp(str1[0], "cat") == 0) {
                                //2 args
                                if (size1 == 2) {
                                        FILE* fptr;
                                        readFile(fptr, str1[1]);
                                }
                                //> Redirection operator (more than 2 args)
                                else if ((strcmp(str1[1], ">")  == 0) && (size1 % 2 != 0) && (size1 > 2)) {
                                        FILE* fptr;
                                        writeFile(fptr, str1[2]);
                                }
                                //>> Redirection operator (more than 2 args)
                                else if ((strcmp(str1[1], ">>")  == 0) && (size1 % 2 != 0) && (size1 > 2)) {
                                        FILE* fptr;
                                        writeFile1(fptr, str1[2]);
                                }

                        }
                }
        }
        //if (strcmp(input, "ls")) {
        //      printf("The input is ls.");
        //}

        //else {
        //      printf("Input is not ls :o");
        //}

        return 1;
}