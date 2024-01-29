#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

unsigned long lcg(unsigned long *seed) {
    const unsigned long a = 22695477;
    const unsigned long c = 1;
    const unsigned long m = 2147483648;

    *seed = (a * (*seed) + c) % m;
    return *seed;
}

int genRand(int min, int max) {
    static unsigned long seed = 0; 

    if (seed == 0) {
        seed = time(NULL);
    }

    return (lcg(&seed) % (max - min + 1)) + min;
}

int isDigitString(const char *str) {
    while (*str) {
        if (!isdigit((unsigned char)*str)) {
            return 0; 
        }
        str++;
    }
    return 1; 
}

int fileExists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return 1; 
    }
    return 0; 
}

int main(int argc, char *argv[]) {
    unsigned long seed = time(NULL); 

    if(argc != 3 && argc != 4) {
        printf("Please provide at least 2 arguments including a number and a file name or 3 arguments with writing mode\n");
        return -1;
    }
   
    if(!isDigitString(argv[1])){
        printf("Please give a valid number in the first argument");
        return -1;
    }

    int num = atoi(argv[1]);
    char* filename = argv[2];
    char* mode = (char*)malloc(2 * sizeof(char));
    
    if(argc == 4) {
        if(strcmp(argv[3], "-a") == 0) {
            mode = "-a";
        }else {
            printf("Please provide valid argument ('-a' or nothing)\n");
            return -1;
        }
    }
    
    // if mode is -a
    if(strcmp(mode,"-a") == 0) {
        if(fileExists(filename) == 0) {
            printf("File not exists\n");
            return -1;
        }

        FILE *file = fopen(filename, "a");
        if(file == NULL) {
            printf("Error opening file\n");
            return -1;
        }

        for(int i = 0; i < num; i++) {          
           fprintf(file, "%d ", genRand(1,100));
        }
        fprintf(file, "\n");
        
        fclose(file);
    }

    // mode is not -a
    else {
        FILE *file = fopen(filename, "w");
        if(file == NULL) {
            printf("Error opening file\n");
            return -1;
        }

        for(int i = 0; i < num; i++) {          
            fprintf(file, "%d ", genRand(1,100));
        }
        fprintf(file, "\n");
        
        fclose(file);
    }

    free(mode);
    return 0;

}