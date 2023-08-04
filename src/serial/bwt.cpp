#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../utils/read_file.h"
#include "../utils/preprocessing.h"
#include "../utils/bwt.h"

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

        genome = addTermination(genome);

        int genome_size = strlen(genome);
        int pattern_size = strlen(pattern);

        clock_t checkpoint = clock();
        double time_spent = (double)(checkpoint - begin) / CLOCKS_PER_SEC;
        printf("Execution time (preprocessing): %.3fs\n", time_spent);

        // Computes the suffix array
        int *suffix_arr = (int *)malloc(genome_size * sizeof(int));
        suffix_arr = computeSuffixArray(genome, genome_size);

        // Adds to the output array the last char
        // of each rotation
        char *bwt_arr = findLastChar(genome, suffix_arr, genome_size);

        int matchIndex = bwtSearch(bwt_arr, suffix_arr, genome_size, pattern, pattern_size, 0);
        if (matchIndex != -1)
        {
            printf("Found match at index %d\n", matchIndex);
        }
        else
        {
            printf("No match found!\n");
        }

        clock_t end = clock();
        time_spent = (double)(end - checkpoint) / CLOCKS_PER_SEC;
        printf("Execution time (search): %.3fs\n", time_spent);

        time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Execution time (total): %.3fs\n", time_spent);

        return 0;
    }
}
