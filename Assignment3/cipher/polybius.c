#include <stdio.h>
#include "table.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *pbEncode(const char *plaintext, table_t *table)
{
    char *res = (char *)malloc(strlen(plaintext) * 2 * sizeof(char) + 1);
    if (res == NULL)
    {
        return NULL;
    }

    char *resPointer = res;
    while (*plaintext != '\0')
    {
        if (!isalpha(*plaintext))
        {
            *resPointer++ = *plaintext++;
        }
        else
        {
            char *encodedChar = encodeChar(plaintext, table);
            strcpy(resPointer, encodedChar);
            resPointer += strlen(encodedChar);
            free(encodedChar);
            plaintext++;
        }
    }
    *resPointer = '\0';

    return res;
}

char *pbDecode(const char *ciphertext, table_t *table)
{
    char *decodeText = (char *)malloc(strlen(ciphertext) * sizeof(char) + 1);
    if (decodeText == NULL)
    {
        return NULL;
    }

    char *decodeTextPointer = decodeText;
    while (*ciphertext != '\0')
    {
        if (!isdigit(*ciphertext))
        {
            *decodeTextPointer++ = *ciphertext++;
        }
        else
        {
            if (*(ciphertext + 1) == '\0')
            {
                free(decodeText);
                return NULL;
            }

            char i = *ciphertext++;
            char j = *ciphertext++;
            *decodeTextPointer = table->charMap[i - '0' - 1][j - '0' - 1];
            decodeTextPointer++;
        }
    }
    *decodeTextPointer = '\0';
    return decodeText;
}
