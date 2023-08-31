# OpenMPI implementation of Burrows-Wheeler Transform for substring matching applied to genomics
The aim of this project is to provide an efficient parallel implementation of a substring matching algorithm and apply it to the search of genes within the genome of an organism.


In this repository you can find the code for the serial and parallel (OpenMPI) implementation in C language.

## Instructions

### 1. Install OpenMPI
Download from the official [Open MPI website](https://www.open-mpi.org/software/ompi/v4.1/) and follow the [official guide](https://www.open-mpi.org/faq/?category=building#easy-build) to install it. This project has been developed using Open MPI 4.1.5, but you can consider working with your preferred compatible version.

### 2. Download the datasets
The datasets can be taken from the [NCBI database](https://www.ncbi.nlm.nih.gov/genbank/). After downloading the genome and genes files of your interest, you should copy and paste the gene (pattern) you want to search in a separate .txt file.


In the experiments performed by the author the following datasets have been used:
* [**Escherichia Coli**](https://www.ncbi.nlm.nih.gov/datasets/taxonomy/562/): a bacteria commonly found in the lower intestine of warm-blooded organisms, 4.6MB
* [**Entamoeba Invadens**](https://www.ncbi.nlm.nih.gov/datasets/taxonomy/33085/): an amoebozoa parasite of reptiles, 40.9MB
* [**Formica Exsecta**](https://www.ncbi.nlm.nih.gov/datasets/taxonomy/72781/): a species of ant found from Western Europe to Asia, 277.6MB

### 3. Compile and Run the Program
#### Serial
To compile and execute the serial version of the program move inside the /src/serial directory and execute the following commands:
```bash
$ gcc main.cpp
$ ./a.out [genomePath] [patternPath]
```
#### Parallel
To compile and execute the parallel version of the program move inside the /src/parallel directory and execute the following commands:
```bash
$ mpicc main.cpp
$ mpirun -n [nodes] a.out [genomePath] [patternPath]
```
