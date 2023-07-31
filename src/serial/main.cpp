#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../utils/read_file.h"

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

char *preprocessing(char *text)
{
    int N = strlen(text);

    char *newString = (char *)malloc(N);
    int j = 0;

    for (int i = 1; i < N; i++)
    {
        if (text[i] != '\n' && (text[i] == 'A' || text[i] == 'T' || text[i] == 'C' || text[i] == 'G'))
        {
            newString[j++] = text[i];
        }
    }

    return newString;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    { // Check that the number of inserted parameters is correct
        printf("Please insert all the necessary parameters");
        return EXIT_FAILURE;
    }
    else
    {
        clock_t begin = clock();

        char *genome = read_file(argv[1]);
        char *pattern = read_file(argv[2]);

        genome = preprocessing(genome);
        pattern = preprocessing(pattern);

        int N = strlen(genome);
        int M = strlen(pattern);

        // create lps[] that will hold the longest prefix suffix
        // values for pattern
        int *lps = (int *)malloc(M);

        // Preprocess the pattern (calculate lps[] array)
        computeLPSArray(pattern, M, lps);

        // Prints occurrences of pattern[] in genome[]
        KMPSearch(pattern, M, genome, N, lps);

        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Execution time (s): %f\n", time_spent);
    }
}