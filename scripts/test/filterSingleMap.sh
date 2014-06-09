####### chemical noise filtering - works for single MS1 maps only #######
PROJECTDIR=$(pwd)
sudo mkdir $PROJECTDIR/tmp1
####### gaussian smoothing with mzscale=1.3 and background removal with structuring element 35 in RT and 25 in mz #######
sudo $PROJECTDIR/findMFHCSout/filterSingleMap --mzscale=1.3 --resolution=35000 --minMass=200 --nrthreads=2 --width-RT=35 --width-MZ=25 --rtscale=1.3 --in=$PROJECTDIR/submodule/findMFData/dil_6_2.mzML --outdir=$PROJECTDIR/tmp
##### setting -width-RT=0 will disable background removal.
sudo mkdir $PROJECTDIR/tmp2
sudo $PROJECTDIR/findMFHCSout/filterSingleMap --mzscale=1.3 --resolution=35000 --minMass=200 --nrthreads=2 --width-RT=0 --width-MZ=0 --rtscale=1.3 --in=$PROJECTDIR/submodule/findMFData/dil_6_2.mzML --outdir=$PROJECTDIR/tmp2
##### disable filtering with enable=0
sudo mkdir $PROJECTDIR/tmp3
$PROJECTDIR/findMFHCSout/filterSingleMap --mzscale=1.3 --resolution=35000 --minMass=200 --nrthreads=2 --enable=0 --in=$PROJECTDIR/submodule/findMFData/dil_6_2.mzML --outdir=$PROJECTDIR/tmp3

