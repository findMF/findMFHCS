#unzip $PROJECTDIR/submodule/findMFData/dil_6_2.mzML.zip -d  $PROJECTDIR/submodule/findMFData/
mkdir $PROJECTDIR/tmp

####### Feature finding #######
# example with feature detection in file with single map. Output is an dil_6_2.sqlite file
$PROJECTDIR/findMFHCSout/findMF --mzscale=1.3 --resolution=65000 --minMass=200 --nrthreads=2 --minintensity=5. --in=$PROJECTDIR/submodule/findMFData/dil_6_2.mzML --outdir=$PROJECTDIR/tmp
# example of feature detection in file with many maps. Output is a file named testS.sqlite
$PROJECTDIR/findMFHCSout/findMF --mzscale=1.3 --resolution=65000 --minMass=200 --nrthreads=2 --minintensity=5. --in=$PROJECTDIR/submodule/findMFData/testS.mzML --outdir=$PROJECTDIR/tmp

####### chemical noise filtering - works for single maps only #######
$PROJECTDIR/findMFHCSout/filterSingleMap --mzscale=1.3 --resolution=65000 --minMass=200 --nrthreads=2 --in=$PROJECTDIR/submodule/findMFData/dil_6_2.mzML --outdir=$PROJECTDIR/tmp

####### Peak picking #######
# intensity
$PROJECTDIR/findMFHCSout/PeakPicker --in $PROJECTDIR/submodule/findMFData/dil_6_2.mzML --out $PROJECTDIR/tmp/dil_6_2.a.c.mzML --resolution=30000 --smoothwidth=2.5 --intwidth=2 --area=0
# area
$PROJECTDIR/findMFHCSout/PeakPicker --in $PROJECTDIR/submodule/findMFData/dil_6_2.mzML --out $PROJECTDIR/tmp/dil_6_2.i.c.mzML --resolution=30000 --smoothwidth=2.5 --intwidth=2 --area=1
####### generating images #######
$PROJECTDIR/findMFHCSout/conv2Image --in $PROJECTDIR/submodule/findMFData/testS.mzML --outdir=$PROJECTDIR/tmp --enable=0

####### visualization #####
$PROJECTDIR/findMFHCSout/filterQTView --in /home/witold/prog/qttest/findMFData/testS.mzML --minintensity=1 --width-RT=20
$PROJECTDIR/findMFHCSout/filterQTView --in /home/witold/prog/qttest/findMFData/dil_6_2.mzML --minintensity=1 --width-RT=20
