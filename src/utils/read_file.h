#include <stdio.h>
#include <stdlib.h>

char *readFile(char *filename)
{
    FILE *file;
    char *buffer = NULL; // initialize buffer to NULL
    int bufferSize = 0;
    int i = 0;

    // Open file for reading
    file = fopen(filename, "r");

    fseek(file, 0L, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    buffer = (char*)malloc(fileSize);

    // Check if file opened successfully
    if (file == NULL)
    {
        printf("Error: Failed to open file '%s'.\n", filename);
        return NULL;
    }

    // Read file character by character
    int c;
    while ((c = fgetc(file)) != EOF)
    {
        buffer[i] = c;
        i++;
    }

    // Close file
    fclose(file);

    return buffer;
}