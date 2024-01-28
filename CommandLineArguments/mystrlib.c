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

void upper(const char *s, char *res)
{
    int i = 0;
    while (s[i] != '\0')
    {
        res[i] = s[i];
        if (res[i] >= 'a' && res[i] <= 'z')
        {
            res[i] -= 'a' - 'A';
        }
        i++;
    }
    res[i] = '\0';
}

void lower(const char *s, char *res)
{
    int i = 0;
    while (s[i] != '\0')
    {
        res[i] = s[i];
        if (res[i] >= 'A' && res[i] <= 'Z')
        {
            res[i] += 'a' - 'A';
        }
        i++;
    }
    res[i] = '\0';
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

int my_strlen(const char *s)
{
    int i = 0;
    while (s[i] != '\0')
    {
        i++;
    }
    return i;
}

void string_concatenate(char *dest, char *s)
{

    while (*dest)
    {
        dest++;
    }

    while (*s)
    {
        *dest = *s;
        dest++;
        s++;
    }

    *dest = '\0';
}