#include <stdio.h>
#include <stdlib.h>

char *read_file(char *filename)
{
    FILE *file;
    char *buffer = NULL; // initialize buffer to NULL
    int buffer_size = 0;
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
        if (i >= buffer_size)
        {
            buffer_size += 1000;                           // increase buffer size by 1000 bytes
            buffer = (char *)realloc(buffer, buffer_size); // resize buffer
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

    // Print the character array
    // printf("%s", buffer);

    // Free the dynamically allocated buffer
    // free(buffer);

    return buffer;
}