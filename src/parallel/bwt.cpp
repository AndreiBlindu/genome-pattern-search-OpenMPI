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
            if (strcmp(argv[3], "--v") == 0 || strcmp(argv[3], "-verbose") == 0)
            {
                VERBOSE = true;
            }
        }

        MPI_Init(&argc, &argv);

        int RANK, SIZE;
        MPI_Comm_rank(MPI_COMM_WORLD, &RANK);
        MPI_Comm_size(MPI_COMM_WORLD, &SIZE);
        double startTime = MPI_Wtime(); // start timer

        char *genome;
        char *pattern;

        long genome_size = 0;
        long pattern_size = 0;

        // Only MASTER reads the files
        if (RANK == 0)
        {
            // read data from files
            genome = read_file(argv[1]);
            pattern = read_file(argv[2]);

            // preprocessing
            genome = preprocessing(genome);
            pattern = preprocessing(pattern);

            genome_size = strlen(genome);
            pattern_size = strlen(pattern);
        }

        // MASTER communicates genome and pattern size via broadcast
        // This is necessary because SLAVES must know how much memory to allocate
        MPI_Bcast(&genome_size, 1, MPI_LONG, 0, MPI_COMM_WORLD);
        MPI_Bcast(&pattern_size, 1, MPI_LONG, 0, MPI_COMM_WORLD);
        if (RANK != 0)
        {
            if (VERBOSE)
            {
                printf("[MPI process %d] Received genome size: %lu\n", RANK, genome_size);
                printf("[MPI process %d] Received pattern size: %lu\n", RANK, pattern_size);
            }

            // SLAVE must allocate memory before receiving the data
            // otherwise we get memory errors
            pattern = (char *)malloc(pattern_size);
        }
        MPI_Barrier(MPI_COMM_WORLD);

        long chunk_size = genome_size / (SIZE - 1);
        long chunk_size_extended = chunk_size + pattern_size;
        char *chunk = (char *)malloc(chunk_size_extended);
        long chunk_start_index = 0;

        // MASTER broadcasts pattern to all slaves
        MPI_Bcast(pattern, pattern_size, MPI_CHAR, 0, MPI_COMM_WORLD);
        if (RANK != 0)
        {
            if (VERBOSE)
            {
                printf("[MPI process %d] Received pattern: %s\n", RANK, pattern);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);

        double endTime1 = MPI_Wtime();

        // MASTER sends to all slaves genome chunks + start index of each chunk
        if (RANK == 0)
        {
            for (int s = 1; s < SIZE; s++)
            {
                memcpy(chunk, &genome[chunk_size * (s - 1)], chunk_size_extended);
                MPI_Send(chunk, chunk_size_extended, MPI_CHAR, s, TAG_CHUNK, MPI_COMM_WORLD);

                chunk_start_index = chunk_size * (s - 1);
                MPI_Send(&chunk_start_index, 1, MPI_LONG, s, TAG_START_INDEX, MPI_COMM_WORLD);
            }
        }
        else // each SLAVE receive a genome chunk + start index of each chunk
        {
            MPI_Recv(chunk, chunk_size_extended, MPI_BYTE, 0, TAG_CHUNK, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&chunk_start_index, 1, MPI_LONG, 0, TAG_START_INDEX, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (VERBOSE)
            {
                printf("[MPI process %d] Received chunk start index: %lu\n", RANK, chunk_start_index);
            }

            chunk = addTermination(chunk);
            chunk_size_extended++;

            // Computes the suffix array
            int *suffix_arr = (int *)malloc(chunk_size_extended * sizeof(int));
            suffix_arr = computeSuffixArray(chunk, chunk_size_extended);

            // Adds to the output array the last char
            // of each rotation
            char *bwt_arr = findLastChar(chunk, suffix_arr, chunk_size_extended);

            int matchIndex = bwtSearch(bwt_arr, suffix_arr, chunk_size_extended, pattern, pattern_size, chunk_start_index);
            if (matchIndex != -1)
            {
                printf("Found match at index %d\n", matchIndex);
            }
            else
            {
                printf("No match found!\n");
            }
        }

        double endTime = MPI_Wtime();
        double timeOfExecution = endTime - endTime1;
        printf("Execution time: %.3fs \n", timeOfExecution);

        MPI_Finalize();
    }
}