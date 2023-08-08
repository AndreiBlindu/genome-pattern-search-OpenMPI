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
        clock_t startTimer = clock();

        char *genome = readFile(argv[1]);
        char *pattern = readFile(argv[2]);

        clock_t readfile = clock();
        double executionTime = (double)(readfile - startTimer) / CLOCKS_PER_SEC;
        printf("Execution time (read file): %.3fs\n", executionTime);

        genome = preprocessing(genome);
        pattern = preprocessing(pattern);

        genome = addTermination(genome);

        int genomeSize = strlen(genome);
        int patternSize = strlen(pattern);

        clock_t checkpoint = clock();
        executionTime = (double)(checkpoint - readfile) / CLOCKS_PER_SEC;
        printf("Execution time (preprocessing): %.3fs\n", executionTime);

        // Computes the suffix array
        int *suffixArr = (int *)malloc(genomeSize * sizeof(int));
        suffixArr = computeSuffixArray(genome, genomeSize);

        // Adds to the output array the last char
        // of each rotation
        char *bwtArr = findLastChar(genome, suffixArr, genomeSize);

        int matchIndex = bwtSearch(bwtArr, suffixArr, genomeSize, pattern, patternSize, 0);
        if (matchIndex != -1)
        {
            printf("Found match at index %d\n", matchIndex);
        }
        else
        {
            printf("No match found!\n");
        }

        clock_t stopTimer = clock();
        executionTime = (double)(stopTimer - checkpoint) / CLOCKS_PER_SEC;
        printf("Execution time (search): %.3fs\n", executionTime);

        executionTime = (double)(stopTimer - startTimer) / CLOCKS_PER_SEC;
        printf("Execution time (total): %.3fs\n", executionTime);

        return 0;
    }
}
