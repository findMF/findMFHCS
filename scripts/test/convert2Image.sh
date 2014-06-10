export PROJECTDIR=$(pwd)
mkdir $PROJECTDIR/images

####### generating images #######
$PROJECTDIR/findMFHCSout/conv2Image --in $PROJECTDIR/submodule/findMFData/testS.mzML --outdir=$PROJECTDIR/images --enable=0
