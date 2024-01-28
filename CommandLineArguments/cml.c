#include <stdio.h>

int my_strcmp(const char *s1, const char *s2)
{
    while (*s1 == *s2)
    {
        if (*s1 == '\0')
        {
            return 0;
        }
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

void capitalize(const char *s, char *res)
{
    int i = 0;
    while (s[i] != '\0')
    {
        res[i] = s[i];
        i++;
    }
    res[i] = '\0';

    if (res[0] >= 'a' && res[0] <= 'z')
    {
        res[0] -= 'a' - 'A';
    }
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
        return -1;
    }
    if (command_checker(argv[1]) == 0)
    {
        printf("Command is only available for following: -caps, -u, -l\n");
        return -1;
    }
    for (int i = 2; i < argc; i++)
    {
        if (my_strcmp(argv[1], "-cap"))
        {
            char[] res = char[sizeof(argv[1])];
            capitalize(res);
            printf("command：%s\n", res);
        }
    }

    return 0;
}