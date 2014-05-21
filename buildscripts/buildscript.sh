workdir=$(pwd)
sudo mkdir findMFHCSbuild
cd findMFHCSbuild

sudo cmake -DFindMFBase_INCLUDE_DIR=$workdir/findMFBase/include -DPwiz_INCLUDE_DIR=$workdir/pwiz/pwiz -DPwiz_LIBRARY=$workdir/pwiz/lib -DVIGRA_LIBRARY=$workdir/vigrabuild/lib -DVIGRA_INCLUDE_DIR=$workdir/vigrabuild/include/ -DFindMFInterfaces_INCLUDE_DIR=$workdir/findMFInterfaces/include ..

sudo make
#sudo make test
sudo ctest -V

