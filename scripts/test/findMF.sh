#unzip $PROJECTDIR/submodule/findMFData/dil_6_2.mzML.zip -d  $PROJECTDIR/submodule/findMFData/

export PROJECTDIR=$(pwd)
sudo mkdir $PROJECTDIR/tmp

####### Feature finding #######
# example with feature detection in file with single map. Output is an dil_6_2.sqlite file
$PROJECTDIR/findMFHCSout/findMF --mzscale=1.3 --resolution=65000 --minMass=200 --nrthreads=2 --minintensity=5. --in=$PROJECTDIR/submodule/findMFData/dil_6_2.mzML --outdir=$PROJECTDIR/tmp
# example of feature detection in file with many maps. Output is a file named testS.sqlite
$PROJECTDIR/findMFHCSout/findMF --mzscale=1.3 --resolution=65000 --minMass=200 --nrthreads=2 --minintensity=5. --in=$PROJECTDIR/submodule/findMFData/testS.mzML --outdir=$PROJECTDIR/tmp

