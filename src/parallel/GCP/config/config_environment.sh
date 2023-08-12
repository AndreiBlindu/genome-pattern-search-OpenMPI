## Access with 
## ssh -i ./id_rsa -l andreiblindu [INSTANCE_EXTERNAL_IP]

# install useful libraries
sudo apt update
sudo apt install wget perl gcc make nano build-essential git

# install OpenMPI
wget https://download.open-mpi.org/release/open-mpi/v4.1/openmpi-4.1.5.tar.gz
gunzip -c openmpi-4.1.5.tar.gz | tar xf -
cd openmpi-4.1.5
sudo ./configure --prefix=/usr/local
sudo make all install
echo "export PATH=\$PATH:/usr/local/openmpi/bin" >>$HOME/.bashrc
echo "export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:/usr/local/openmpi/lib" \  >>$HOME/.bashrc
cd ..
sudo rm -r openmpi-4.1.5
rm openmpi-4.1.5.tar.gz
sudo ldconfig

## ALL INSTANCES MUST HAVE THE KEY PAIR
## sudo scp -i ./id_rsa ./id_rsa* andreiblindu@34.136.194.57:/home/andreiblindu/.ssh/

## TO MOVE FILES TO REMOTE (from folder ./myGoogleKeys)
## sudo scp -i ./id_rsa "/Users/andreiblindu/Desktop/Università/Laurea Magistrale/Corsi/1° anno/1° semestre/Advanced Computer Architecture/Project/genome-pattern-search-OpenMPI/datasets/FormicaExsecta/genome.fna" andreiblindu@35.238.84.102:/home/andreiblindu/datasets/FormicaExsecta
## sudo scp -i ./id_rsa "/Users/andreiblindu/Desktop/Università/Laurea Magistrale/Corsi/1° anno/1° semestre/Advanced Computer Architecture/Project/genome-pattern-search-OpenMPI/datasets/FormicaExsecta/pattern.txt" andreiblindu@35.238.84.102:/home/andreiblindu/datasets/FormicaExsecta