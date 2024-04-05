#define CHAR_MAP_SIZE 5
#define ENCODE_SIZE 26

typedef struct
{
    /* data */
    char charMap[CHAR_MAP_SIZE][CHAR_MAP_SIZE];
    int encodeMap[ENCODE_SIZE];
} table_t;

void initializeTable(table_t *table);

char *encodeChar(const char *char2encode, table_t *table);