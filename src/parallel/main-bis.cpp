#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/read_file.h"
#include "../utils/preprocessing.h"
#include "../utils/bwt.h"

// export PATH="/Users/andreiblindu/mpi/bin:$PATH"
// mpicc -o [exec_name] [source_name] <-- compile
// mpirun -n [nodes] [exec_name] <-- run

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

        int genomeSize = 0;
        int patternSize = 0;

        // Only MASTER reads the files
        if (RANK == 0)
        {
            // read data from files
            genome = readFile(argv[1]);
            pattern = readFile(argv[2]);

            // preprocessing (only pattern)
            pattern = preprocessing(pattern);

            genomeSize = strlen(genome);
            patternSize = strlen(pattern);
        }

        double readfileEnd = MPI_Wtime();
        double executionTime = readfileEnd - startTimer;

        if (RANK == 0)
        {
            printf("Execution time (read file): %.3fs \n", executionTime);
        }

        // MASTER communicates genome size via broadcast
        // This is necessary because SLAVES must know how much memory to allocate
        MPI_Bcast(&genomeSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (RANK != 0)
        {
            if (VERBOSE)
            {
                printf("[MPI process %d] Received genome size: %d\n", RANK, genomeSize);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);

        int chunkSize = genomeSize / SIZE;
        char *chunk = (char *)malloc(chunkSize);

        // MASTER sends to all slaves genome chunks for preprocessing
        MPI_Scatter(genome, chunkSize, MPI_CHAR, chunk, chunkSize, MPI_CHAR, 0, MPI_COMM_WORLD);

        // all SLAVES and MASTER preprocess their chunk
        chunk = preprocessing(chunk);
        chunkSize = strlen(chunk);

        // Since the chunks have been preprocessed now they have different sizes so MPI_Gather does not
        // work and we need to use MPI_Gatherv, that requires the MASTER process to know the displacements
        // relative to each chunk but they are not known a priori since they depend on the chunk size after
        // the preprocessing. That's why each node sends to the MASTER each chunkSize after the preprocessing
        // so the MASTER can compute the displacement before calling MPI_Gatherv.
        int *chunkSizesList = (int*)malloc(SIZE);
        int *displ = (int*)malloc(SIZE);
        MPI_Gather(&chunkSize, 1, MPI_INT, chunkSizesList, 1, MPI_INT, 0, MPI_COMM_WORLD);

        int totGenSize = 0;
        if (RANK == 0)
        {
            for (int i=0; i<SIZE; i++)
            {
                displ[i] = 0;
                for (int j=0; j < i; j++)
                {
                    displ[i] += chunkSizesList[j];
                }
                totGenSize += chunkSizesList[i];
            }

            genome = (char*)malloc(totGenSize);
        }
        MPI_Barrier(MPI_COMM_WORLD);

        // and then send the preprocessed chunk to master
        MPI_Gatherv(chunk, chunkSize, MPI_CHAR, genome, chunkSizesList, displ, MPI_CHAR, 0, MPI_COMM_WORLD);

        if (RANK == 0)
        {
            // recalculates genomeSize after preprocessing
            genomeSize = strlen(genome);
        }

        double preprocessingEnd = MPI_Wtime();
        executionTime = preprocessingEnd - readfileEnd;
        printf("[%d] Execution time (preprocessing): %.3fs \n", RANK, executionTime);

        // MASTER communicates genome and pattern size via broadcast
        // This is necessary because SLAVES must know how much memory to allocate
        MPI_Bcast(&genomeSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&patternSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (RANK != 0)
        {
            if (VERBOSE)
            {
                printf("[MPI process %d] Received genome size: %d\n", RANK, genomeSize);
                printf("[MPI process %d] Received pattern size: %d\n", RANK, patternSize);
            }

            // SLAVE must allocate memory before receiving the data
            // otherwise we get memory errors
            pattern = (char *)malloc(patternSize);
        }
        MPI_Barrier(MPI_COMM_WORLD);

        chunkSize = genomeSize / SIZE;
        int chunkSizeExtended = chunkSize + patternSize;
        chunk = (char *)malloc(chunkSizeExtended);
        int chunkStartIndex = 0;

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
                MPI_Send(&chunkStartIndex, 1, MPI_INT, s, TAG_START_INDEX, MPI_COMM_WORLD);
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
            MPI_Recv(chunk, chunkSizeExtended, MPI_BYTE, 0, TAG_CHUNK, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&chunkStartIndex, 1, MPI_INT, 0, TAG_START_INDEX, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (VERBOSE)
            {
                printf("[MPI process %d] Received chunk start index: %d\n", RANK, chunkStartIndex);
            }
        }

        chunk = addTermination(chunk);
        chunkSizeExtended++;

        // Computes the suffix array
        int *suffixArr = (int *)malloc(chunkSizeExtended * sizeof(int));
        suffixArr = computeSuffixArray(chunk, chunkSizeExtended);

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