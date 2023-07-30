#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fills lps[] for given pattern pat[0..M-1]
void computeLPSArray(char *pat, int M, int *lps)
{
    // length of the previous longest prefix suffix
    int len = 0;

    lps[0] = 0; // lps[0] is always 0

    // the loop calculates lps[i] for i = 1 to M-1
    int i = 1;
    while (i < M)
    {
        if (pat[i] == pat[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else // (pat[i] != pat[len])
        {
            if (len != 0)
            {
                len = lps[len - 1];
            }
            else // if (len == 0)
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

void KMPSearch(char *pat, int M, char *txt, int N, int *lps)
{
    int i = 0; // index for txt[]
    int j = 0; // index for pat[]
    while ((N - i) >= (M - j))
    {
        if (pat[j] == txt[i])
        {
            j++;
            i++;
        }

        if (j == M)
        {
            printf("Found pattern at index %d \n", i - j);
            j = lps[j - 1];
        }

        // mismatch after j matches
        else if (i < N && pat[j] != txt[i])
        {
            // Do not match lps[0..lps[j-1]] characters,
            // they will match anyway
            if (j != 0)
                j = lps[j - 1];
            else
                i = i + 1;
        }
    }
}

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

int main(int argc, char** argv)
{
    if (argc < 3)
    { // Check that the number of inserted parameters is correct
        printf("Please insert all the necessary parameters");
        return EXIT_FAILURE;
    }
    else
    {
        char *txt = read_file(argv[1]);
        char *pat = read_file(argv[2]);

        int N = strlen(txt);
        int M = strlen(pat);

        // create lps[] that will hold the longest prefix suffix
        // values for pattern
        int *lps = (int *)malloc(M);

        // Preprocess the pattern (calculate lps[] array)
        computeLPSArray(pat, M, lps);

        // Prints occurrences of txt[] in pat[]
        KMPSearch(pat, M, txt, N, lps);
    }
}