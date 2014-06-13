export PROJECTDIR=$(pwd)

# prepare the data file
sudo unzip $PROJECTDIR/submodule/findMFData/dil_6_2.mzML.zip -d $PROJECTDIR/submodule/findMFData/
sudo $PROJECTDIR/scripts/test/pp2.sh
sudo $PROJECTDIR/scripts/test/filterSingleMap.sh
#sudo $PROJECTDIR/scripts/test/findMF.sh
