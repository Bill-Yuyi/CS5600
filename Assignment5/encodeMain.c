#include <stdio.h>
#include "table.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "polybius.h"

// children process for encoding
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    FILE *outputFile = fopen(argv[1], "w");
    if (!outputFile)
    {
        perror("Error opening output file");
        return EXIT_FAILURE;
    }

    // initialize encoding table
    table_t *table = (table_t *)malloc(sizeof(table_t));
    if (table == NULL)
    {
        return -1;
    }
    initializeTable(table);

    // start to encode
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        char *res = pbEncode(buffer, table);
        // printf("%s", res);
        fputs(res, outputFile);
        fflush(outputFile);
        free(res);
    }
    free(table);

    fclose(outputFile);
    return EXIT_SUCCESS;
}
