# download git if not installed
yum install git

# download repo from git
git clone https://github.com/AndreiBlindu/genome-pattern-search-OpenMPI.git

cd genome-pattern-search-OpenMPI/src/parallel

## mpirun --use-hwthread-cpus -n 2 a.out ../../datasets/EscherichiaColi/genome.fna ../../datasets/EscherichiaColi/pattern.txt
## mpirun --use-hwthread-cpus -n 2 a.out /home/andreiblindu/datasets/FormicaExsecta/genome.fna /home/andreiblindu/datasets/FormicaExsecta/pattern.txt

10.128.0.2 slots=2
10.128.0.3 slots=2
10.128.0.5 slots=2
10.128.0.6 slots=2
10.128.0.7 slots=2
10.128.0.8 slots=2