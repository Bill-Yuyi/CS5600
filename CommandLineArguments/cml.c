#include <stdio.h>
#include "mystrlib.h"
int command_checker(char *command)
{
    // printf("command：%s\n", command);
    if (my_strcmp(command, "-cap") != 0 && my_strcmp(command, "-u") != 0 && my_strcmp(command, "-l") != 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int main(int argc, char *argv[])
{
    char result[1000] = "";

    if (argc < 3)
    {
        printf("Please provide at least 2 args\n");
        return -1;
    }

    if (command_checker(argv[1]) == 0)
    {
        printf("Command is only available for following: -caps, -u, -l\n");
        return -1;
    }

    for (int i = 2; i < argc; i++)
    {
        char res[my_strlen(argv[i]) + 1];
        if (my_strcmp(argv[1], "-cap") == 0)
        {
            capitalize(argv[i], res);
            if (i != 2)
            {
                string_concatenate(result, " ");
            }
            string_concatenate(result, res);
        }

        else if (my_strcmp(argv[1], "-u") == 0)
        {
            upper(argv[i], res);
            if (i != 2)
            {
                string_concatenate(result, " ");
            }
            string_concatenate(result, res);
        }
        else
        {

            lower(argv[i], res);
            if (i != 2)
            {
                string_concatenate(result, " ");
            }
            string_concatenate(result, res);
        }
    }

    printf("%s\n", result);

    return 0;
}