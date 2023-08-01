#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../utils/read_file.h"
#include "../utils/preprocessing.h"
#include "../utils/kmp.h"

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