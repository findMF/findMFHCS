workdir=$(pwd)
sudo mkdir findMFHCSbuild
cd findMFHCSbuild

sudo cmake -DFindMFBase_INCLUDE_DIR=$workdir/findMFBase/include -DPwiz_INCLUDE_DIR=$workdir/pwiz/pwiz -DPwiz_LIBRARY=$workdir/pwiz/lib -DVIGRA_INCLUDE_DIR=$workdir/vigrabuild/lib -VIGRA_INCLUDE_DIR=$workdir/vigrabuild/include ..
sudo make 
sudo make test

