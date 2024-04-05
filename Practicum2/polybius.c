#include <stdio.h>
#include "table.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Encodes a given plaintext string using a Polybius square table.
 *
 * The function iterates through each character of the input plaintext. If the character
 * is an alphabetical letter, it is encoded using the provided Polybius square table,
 * resulting in a two-digit number that represents the row and column in the table
 * where the letter is found. Non-alphabetical characters are copied directly to the
 * result string without encoding.
 *
 * @param plaintext The input string to be encoded.
 * @param table A pointer to a `table_t` structure that represents the Polybius square
 *              table used for encoding the letters.
 * @return A pointer to the dynamically allocated encoded string.
 */

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

/**
 * Decodes a given ciphertext string using a Polybius square table.
 *
 * The function iterates through each character of the ciphertext. If two consecutive characters
 * are digits and each digit is within the range of '1' to '5', they are decoded to
 * the corresponding alphabetical letter. Non-digit characters and digits that do not form
 * a valid two-digit number are copied directly to the result string without decoding.
 *
 * @param ciphertext The input string to be decoded.
 * @param table A pointer to a `table_t` structure that represents the Polybius square
 *              table used for decoding the numbers back to letters.
 * @return A pointer to the dynamically allocated decoded string.
 */

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
            // if current char is not digit, just copy that into decode text
            *decodeTextPointer++ = *ciphertext++;
        }
        else
        {
            // check if we have two digits in row in order to decode
            if (*(ciphertext + 1) != '\0' && isdigit(*(ciphertext + 1)))
            {
                char i = *ciphertext++;
                char j = *ciphertext++;

                // digits should be in the range
                if (i >= '1' && i <= '5' && j >= '1' && j <= '5')
                {
                    *decodeTextPointer++ = table->charMap[i - '0' - 1][j - '0' - 1];
                }
                else
                {
                    // if out of range, we just copy
                    *decodeTextPointer++ = i;
                    *decodeTextPointer++ = j;
                }
            }
            else
            {
                // we dont have two digits to decode, so just copy
                *decodeTextPointer++ = *ciphertext++;
            }
        }
    }
    *decodeTextPointer = '\0';
    return decodeText;
}
