export PROJECTDIR=$(pwd)

# prepare the data file
sudo unzip $PROJECTDIR/submodule/findMFData/dil_6_2.mzML.zip -d $PROJECTDIR/submodule/findMFData/
sudo pp2.sh
