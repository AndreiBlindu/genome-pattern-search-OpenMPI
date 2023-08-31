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