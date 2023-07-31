#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/read_file.h"

// export PATH="/Users/andreiblindu/mpi/bin:$PATH"
// mpicc -o [exec_name] [source_name] <-- compile
// mpirun -n [nodes] [exec_name] <-- run

int main(int argc, char **argv)
{
    if (argc < 3)
    { // Check that the number of inserted parameters is correct
        printf("Please insert all the necessary parameters");
        return EXIT_FAILURE;
    }
    else
    {
        MPI_Init(&argc, &argv);

        int RANK, SIZE;
        MPI_Comm_rank(MPI_COMM_WORLD, &RANK);
        MPI_Comm_size(MPI_COMM_WORLD, &SIZE);
        double startTime = MPI_Wtime(); // start timer

        char *genome;
        char *pattern;

        // Only MASTER reads the files
        if (RANK == 0)
        {
            // read data from files
            genome = read_file(argv[1]);
            pattern = read_file(argv[2]);
        }

        MPI_Finalize();
    }
}