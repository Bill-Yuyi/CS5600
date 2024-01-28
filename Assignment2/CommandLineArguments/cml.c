#include <stdio.h>
#include "mystrlib.h"

int main(int argc, char *argv[])
{
    char result[1000] = "";

    // handle error if args is less than 3
    if (argc < 3)
    {
        printf("Please provide at least 2 args\n");
        return -1;
    }

    // check if the first command is valid
    if (command_checker(argv[1]) == 1)
    {
        printf("Command is only available for following: -caps, -u, -l\n");
        return -1;
    }

    // start to do operation on text after that command
    for (int i = 2; i < argc; i++)
    {
        char res[my_strlen(argv[i]) + 1];
        string_operation(argv[1], argv[i], result, res, i);
    }

    printf("%s\n", result);

    return 0;
}