sudo git clone https://github.com/ukoethe/vigra.git
vigrabuild=$(pwd)/vigrabuild #place products here

echo $vigrabuild

sudo mkdir vigra-build
cd vigra-build

sudo cmake -DCMAKE_INSTALL_PREFIX=$vigrabuild -DWITH_HDF5=OFF -DWITH_VIGRANUMPY=OFF ../vigra
sudo make
sudo make install
