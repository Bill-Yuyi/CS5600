/**
 * Compares two strings.
 *
 * @param s1 Pointer to the first string.
 * @param s2 Pointer to the second string.
 * @return Returns 0 if s1 and s2 are equal; a value less than 0 if s1 is less than s2; a value greater than 0 if s1 is greater than s2.
 */

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

/**
 * Converts a string to uppercase.
 *
 * @param s Pointer to the string to be converted.
 * @param res Pointer to the resulting string. This buffer should be large enough to hold the converted string.
 */
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

/**
 * Converts a string to lowercase.
 *
 * @param s Pointer to the string to be converted.
 * @param res Pointer to the resulting string.
 */
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

/**
 * Capitalizes the first character of a string.
 *
 * @param s Pointer to the string to be capitalized.
 * @param res Pointer to the resulting string.
 */
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

/**
 * Calculates the length of a string.
 *
 * @param s Pointer to the string.
 * @return The length of the string.
 */
int my_strlen(const char *s)
{
    int i = 0;
    while (s[i] != '\0')
    {
        i++;
    }
    return i;
}

/**
 * Concatenates the source string to the end of the destination string.
 *
 * @param dest Pointer to the destination string.
 * @param s Pointer to the source string.
 */
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

/**
 * Checks if a given command is valid.
 *
 * @param command The command string to check.
 * @return Returns 0 if the command is valid, otherwise 1.
 */
int command_checker(char *command)
{
    if (my_strcmp(command, "-cap") != 0 && my_strcmp(command, "-u") != 0 && my_strcmp(command, "-l") != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}