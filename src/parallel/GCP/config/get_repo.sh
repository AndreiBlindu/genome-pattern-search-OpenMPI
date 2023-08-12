# download git if not installed
sudo apt install git

# download repo from git
git clone https://github.com/AndreiBlindu/genome-pattern-search-OpenMPI.git

cd genome-pattern-search-OpenMPI/src/parallel

## mpirun --use-hwthread-cpus -n 2 a.out ../../datasets/EscherichiaColi/genome.fna ../../datasets/EscherichiaColi/pattern.txt
## mpirun --use-hwthread-cpus -n 2 a.out /home/andreiblindu/datasets/EntamoebaInvadens/genome.fna /home/andreiblindu/datasets/EntamoebaInvadens/pattern.txt
## mpirun --use-hwthread-cpus -n 2 a.out /home/andreiblindu/datasets/FormicaExsecta/genome.fna /home/andreiblindu/datasets/FormicaExsecta/pattern.txt

## I have to move the file a.out out of the directory of the git project because there for some reason I cannot execute it
## so I do it cp a.out /home/andreiblindu/
## Then to run I use
## mpirun --hostfile ./genome-pattern-search-OpenMPI/src/parallel/hostfile -np 2 ./a.out ./genome-pattern-search-OpenMPI/datasets/EscherichiaColi/genome.fna ./genome-pattern-search-OpenMPI/datasets/EscherichiaColi/pattern.txt