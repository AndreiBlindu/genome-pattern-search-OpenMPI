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
        // If buffer is full, resize it
        if (i >= bufferSize)
        {
            bufferSize += 1000;                           // increase buffer size by 1000 bytes
            buffer = (char *)realloc(buffer, bufferSize); // resize buffer
            if (buffer == NULL)
            {
                printf("Error: Memory allocation failed.\n");
                return NULL;
            }
        }
        buffer[i] = c;
        i++;
    }

    // Close file
    fclose(file);

    return buffer;
}