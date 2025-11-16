#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>


#define BUFFER_SIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

int lsh_cd(char** args);
int lsh_exit(char** args);
int lsh_help(char** args);



char* functions_arr[] = {
    "cd","exit","help"
};
int lsh_num_builtins() {
  return sizeof(functions_arr) / sizeof(char *);
}
int (*builtin_func[]) (char **) = {
    &lsh_cd,&lsh_exit,&lsh_help
};

void die(char* s) {
    perror(s);
    exit(EXIT_FAILURE);
}
char* lsh_read_line() {
    int bufsize = BUFFER_SIZE;
    int pos = 0; 
    char* buffer = malloc(sizeof(char)*bufsize);
    int c; 

    if(!buffer) {
        die("Insufficient memory allocated");
    }
    
    while(1) {
        c = getchar();
        if((c == EOF) || (c == '\n')) {
            buffer[pos] = '\0';
            return buffer;
        }
        buffer[pos] = c;
        pos++;

        if (pos>=bufsize) {
            bufsize += BUFFER_SIZE;
            buffer = realloc(buffer,bufsize);
            if(!buffer) {
                die("Realloc: Memory allocation failed");
            }
        }
    }
}


char** lsh_split_line(char* line) {
    int bufsize = LSH_TOK_BUFSIZE;  // represents total number of args.
    int pos = 0;
    char** tokens = malloc(sizeof(char*)*bufsize);
    char* token;
    if(!tokens) {
        die("Tokens: insufficient memory allocation");
    }
    token = strtok(line,LSH_TOK_DELIM);
    while(token!=NULL) {
        tokens[pos] = token;
        pos++;

        if(pos>=bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens,bufsize);
            if(!tokens) {
                die("Realloc: Memory allocation failed");
            }
        }
        token = strtok(NULL,LSH_TOK_DELIM);
    }
    token[pos] = NULL;
    return tokens;
}

int lsh_launch(char** args) {
    pid_t pid,wpid;
    int status;

    pid = fork();
    if(pid == 0) {
        if(execvp(args[0],args)==-1) die("failed to reload execution state");
        return EXIT_FAILURE;
    }
    else if (pid < 0) {
        die("Error forking process");
    }
    else {
        do{
            wpid = waitpid(pid,&status,WUNTRACED);
        }while(!(WIFEXITED(status)) && !(WIFSIGNALED(status)));
    }
    return 1;
}

void lsh_loop(void) {

    char *line;
    char **args;
    int status; 

    do {
        printf("> ");
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);
        free(line);
        free(args);
    } while(status);
}







int main(int argc, char* argv[]) {

    lsh_loop();
    return EXIT_SUCCESS;
}