#include <stdio.h>

int my_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

int command_checker(char *command)
{
    printf("command：%s\n", command);
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
    if (argc < 2)
    {
        printf("Please provide at least 2 args");
        return 0;
    }

    for (int i = 1; i < argc; i++)
    {
        if (command_checker(argv[i]) == 0)
        {
            printf("Command is only available for following: -caps, -u, -l\n");
            return 0;
        }
    }

    return 0;
}