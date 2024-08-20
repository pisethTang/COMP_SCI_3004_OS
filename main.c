#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define LSH_RL_BUFFSIZE 1024
#define LSH_TOK_BUFFSIZE 64 
#define LSH_TOK_DELIM " \t\r\n\a"

char* lsh_read_line(void);
char** lsh_split_line(char*);
int lsh_launch(char**);


// The final piece of the puzzle!
int lsh_execute(char**);
/* Function declaration for built-in shell commands:
*/
int lsh_cd(char** args);
int lsh_help(char** args);
int lsh_exit(char** args);

/*
List of built-in commands, followed by their corresponding functionalities.
*/
char* builtin_str[] = {
    "cd",
    "help",
    "exit"
};


int (*builtin_func[])(char**) = {
    &lsh_cd,
    &lsh_help,
    &lsh_exit
};

int lsh_num_builtins(){
    return sizeof(builtin_str)/sizeof(char*);
}

/*Built-in functions implementations*/
int lsh_cd(char** args){
    if (args[1] == NULL){
        fprintf(stderr, "lsh: expected agument to \"cd\"\n");
    }
    else{
        if (chdir(args[1]) != 0) perror("lsh");
    }
    return 1;
}

int lsh_help(char** args){
    int i;
    printf("Seth's LSH");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in: \n");

    for (i=0; i<lsh_num_builtins(); i++){
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs");
    return 1;
}

int lsh_exit(char** args){return 0;} // a signal for the comman loop to terminate 




char* lsh_read_line(void){
    int buff_size = LSH_RL_BUFFSIZE;
    int position = 0;
    char* buffer = malloc(sizeof(char) * buff_size);
    int c;

    if (!buffer){
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        // Read a character 
        c = getchar();
        // If we hit EOF, replace it with a null character and return.
        if (c == EOF || c == '\n'){
            buffer[position] = '\0';
            return buffer;   
        }
        else{
            buffer[position] = c;
        }
        position++;

        // If we have exceeded the buffer, reallocate the buffer
        if (position >= buff_size){
            buff_size = buff_size + LSH_RL_BUFFSIZE;
            buffer = realloc(buffer, buff_size);

            // Checking for reallocation failure 
            if (!buffer){
                fprintf(stderr,"lsh: alllocation error\n");
                exit(EXIT_FAILURE);
            }
        }

    }

    return 0;
}
char** lsh_split_line(char* line){
    int buff_size = LSH_TOK_BUFFSIZE, position  = 0;
    char** tokens = malloc(buff_size * sizeof(char*));
    char* token;

    if (!tokens){
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL){
        tokens[position] = token;
        position++;

        if (position >= buff_size){
            buff_size = buff_size + LSH_TOK_BUFFSIZE;
            tokens = realloc(tokens, buff_size * sizeof(char*));
            if (!tokens){
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens; // Now, we have an array of tokens ready to be executed. 
}

int lsh_launch(char** args){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0){
        // Child process 
        if (execvp(args[0], args) == -1) perror("lsh");
        exit(EXIT_FAILURE);
    }
    else if (pid < 0){
        // Error forking
        perror("lsh"); // Using perror() to print system error messages along with the 
        // the program name so user knows where the error occurred
    }
    else{
        // Parent process 
        do{
            wpid = waitpid(pid, &status, WUNTRACED);
        }while (!WIFEXITED(status) && ! WIFSIGNALED(status));
    }


    return 1; // A signal to the calling function that we should prompt for input again. 
}

int lsh_execute(char** args){
    int i;


    // To handle empty or empty string input from the user
    if (args[0] == NULL){
        // AN empty command was entered
        return 1;
    }

    for (i=0; i<lsh_num_builtins(); i++){
        if (strcmp(args[0], builtin_str[i]) == 0){
            return (*builtin_func[i])(args);
        }
    }

    return lsh_launch(args);
}


void lsh_loop(void){
    char* line;
    char** args;
    int status;

    do{
        printf("> ");
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);
    }while(status);
}

int main(int argc, char* argv[]){
    // Load config files, if any.

    // Run command loop 
    lsh_loop();




    return EXIT_SUCCESS;
}


/*
Source: https://brennan.io/2015/01/16/write-a-shell-in-c/

#include <sys/wait.h>
    waitpid() and associated macros
#include <unistd.h>
    chdir()
    fork()
    exec()
    pid_t
#include <stdlib.h>
    malloc()
    realloc()
    free()
    exit()
    execvp()
    EXIT_SUCCESS, EXIT_FAILURE
#include <stdio.h>
    fprintf()
    printf()
    stderr
    getchar()
    perror()
#include <string.h>
    strcmp()
    strtok()
*/