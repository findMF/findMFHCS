export PROJECTDIR=$(pwd)
sudo mkdir $PROJECTDIR/images

####### generating images #######
sudo $PROJECTDIR/findMFHCSout/conv2Image --in $PROJECTDIR/submodule/findMFData/testS.mzML --outdir=$PROJECTDIR/images --enable=0
