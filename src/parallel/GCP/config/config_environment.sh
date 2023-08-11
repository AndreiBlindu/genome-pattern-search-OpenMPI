## Access with 
## ssh -i ./id_rsa -l andreiblindu [INSTANCE_EXTERNAL_IP]

sudo su

# install useful libraries
yum install wget perl gcc gcc-c++ make nano git

# install OpenMPI
mkdir openMPI
cd openMPI
wget https://download.open-mpi.org/release/open-mpi/v4.1/openmpi-4.1.5.tar.gz
tar -xzvf openmpi-4.1.5.tar.gz
cd openmpi-4.1.5
mkdir build
cd build
../configure --prefix=/usr/local/openMPI
make all install

## add
## export PATH=$PATH:/usr/local/openMPI/bin
## to /home/andreiblindu/.bashrc file

## TO MOVE FILES TO REMOTE (from folder ./myGoogleKeys)
## sudo scp -i ./id_rsa "/Users/andreiblindu/Desktop/Università/Laurea Magistrale/Corsi/1° anno/1° semestre/Advanced Computer Architecture/Project/genome-pattern-search-OpenMPI/datasets/FormicaExsecta/genome.fna" andreiblindu@35.238.84.102:/home/andreiblindu/datasets/FormicaExsecta
## sudo scp -i ./id_rsa "/Users/andreiblindu/Desktop/Università/Laurea Magistrale/Corsi/1° anno/1° semestre/Advanced Computer Architecture/Project/genome-pattern-search-OpenMPI/datasets/FormicaExsecta/pattern.txt" andreiblindu@35.238.84.102:/home/andreiblindu/datasets/FormicaExsecta