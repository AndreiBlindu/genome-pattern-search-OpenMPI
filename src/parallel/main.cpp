#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/read_file.h"
#include "../utils/preprocessing.h"
#include "../utils/bwt.h"

#define TAG_CHUNK 100
#define TAG_START_INDEX 101

int main(int argc, char **argv)
{
    bool VERBOSE = false;

    if (argc < 3)
    { // Check that the number of inserted parameters is correct
        printf("Please insert all the necessary parameters");
        return EXIT_FAILURE;
    }
    else
    {
        if (argc > 3)
        {
            if (strcmp(argv[3], "-v") == 0 || strcmp(argv[3], "--v") == 0 || strcmp(argv[3], "-verbose") == 0)
            {
                VERBOSE = true;
            }
        }

        MPI_Init(&argc, &argv);

        int RANK, SIZE;
        MPI_Comm_rank(MPI_COMM_WORLD, &RANK);
        MPI_Comm_size(MPI_COMM_WORLD, &SIZE);
        double startTimer = MPI_Wtime(); // start timer

        char *genome;
        char *pattern;

        long genomeSize = 0;
        long patternSize = 0;

        // Only MASTER reads the files
        if (RANK == 0)
        {
            // read data from files
            genome = readFile(argv[1]);
            pattern = readFile(argv[2]);

            // preprocessing
            genome = preprocessing(genome);
            pattern = preprocessing(pattern);

            genomeSize = strlen(genome);
            patternSize = strlen(pattern);
        }

        double preprocessingEnd = MPI_Wtime();
        double executionTime = preprocessingEnd - startTimer;

        if (RANK == 0)
        {
            printf("Execution time (read + preprocessing): %.3fs \n", executionTime);
        }

        // MASTER communicates genome and pattern size via broadcast
        // This is necessary because SLAVES must know how much memory to allocate
        MPI_Bcast(&genomeSize, 1, MPI_LONG, 0, MPI_COMM_WORLD);
        MPI_Bcast(&patternSize, 1, MPI_LONG, 0, MPI_COMM_WORLD);
        if (RANK != 0)
        {
            if (VERBOSE)
            {
                printf("[MPI process %d] Received genome size: %lu\n", RANK, genomeSize);
                printf("[MPI process %d] Received pattern size: %lu\n", RANK, patternSize);
            }

            // SLAVE must allocate memory before receiving the data
            // otherwise we get memory errors
            pattern = (char *)malloc(patternSize);
        }
        MPI_Barrier(MPI_COMM_WORLD);

        long chunkSize = genomeSize / SIZE;
        long chunkSizeExtended = chunkSize + patternSize;
        char *chunk = (char *)malloc(chunkSizeExtended);
        long chunkStartIndex = 0;

        // MASTER broadcasts pattern to all slaves
        MPI_Bcast(pattern, patternSize, MPI_CHAR, 0, MPI_COMM_WORLD);
        if (RANK != 0)
        {
            if (VERBOSE)
            {
                printf("[MPI process %d] Received pattern: %s\n", RANK, pattern);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);

        // MASTER sends to all slaves genome chunks + start index of each chunk
        if (RANK == 0)
        {
            for (int s = 1; s < SIZE; s++)
            {
                memcpy(chunk, &genome[chunkSize * s], chunkSizeExtended);
                MPI_Send(chunk, chunkSizeExtended, MPI_CHAR, s, TAG_CHUNK, MPI_COMM_WORLD);

                chunkStartIndex = chunkSize * s;
                MPI_Send(&chunkStartIndex, 1, MPI_LONG, s, TAG_START_INDEX, MPI_COMM_WORLD);
            }

            if (SIZE == 1)
            {
                // if there's only the MASTER node it takes as a chunk the whole genome and
                // chunkSizeExtended is chunkSize since there's no need to consider overlapping
                // with other nodes'chunks and we what to avoid segmentation faults
                chunkSizeExtended = chunkSize;
            }

            // the MASTER works on the first chunks
            memcpy(chunk, &genome[0], chunkSizeExtended);
            chunkStartIndex = 0;
        }
        else // each SLAVE receive a genome chunk + start index of each chunk
        {
            MPI_Recv(chunk, chunkSizeExtended, MPI_CHAR, 0, TAG_CHUNK, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&chunkStartIndex, 1, MPI_LONG, 0, TAG_START_INDEX, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (VERBOSE)
            {
                printf("[MPI process %d] Received chunk start index: %lu\n", RANK, chunkStartIndex);
            }
        }

        chunk = addTermination(chunk);
        chunkSizeExtended++;

        // Computes the suffix array
        //int *suffixArr = (int *)malloc(chunkSizeExtended * sizeof(int));
        int *suffixArr = computeSuffixArray(chunk, chunkSizeExtended);

        // Adds to the output array the last char
        // of each rotation
        char *bwtArr = findLastChar(chunk, suffixArr, chunkSizeExtended);

        int matchIndex = bwtSearch(bwtArr, suffixArr, chunkSizeExtended, pattern, patternSize, chunkStartIndex);
        if (matchIndex != -1)
        {
            printf("[%d] Found match at index %d\n", RANK, matchIndex);
        }
        else
        {
            printf("[%d] No match found!\n", RANK);
        }

        double stopTimer = MPI_Wtime();
        executionTime = stopTimer - preprocessingEnd;
        printf("Execution time (search): %.3fs \n", executionTime);
        executionTime = stopTimer - startTimer;
        printf("Execution time (total): %.3fs \n", executionTime);

        MPI_Finalize();
    }
}