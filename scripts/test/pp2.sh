#unzip $PROJECTDIR/submodule/findMFData/dil_6_2.mzML.zip -d  $PROJECTDIR/submodule/findMFData/
# run this script from project root by:
#./scripts/test/pp.sh
export PROJECTDIR=$(pwd)
mkdir $PROJECTDIR/tmp

####### Peak picking #######
# intensity

$PROJECTDIR/findMFHCSout/PeakPicker --in $PROJECTDIR/submodule/findMFData/testS.mzML --out $PROJECTDIR/tmp/testS.i.c.mzML --resolution=30000 --smoothwidth=2.5 --intwidth=2 --area=1
$PROJECTDIR/findMFHCSout/PeakPicker --in $PROJECTDIR/submodule/findMFData/testS.mzML --out $PROJECTDIR/tmp/testS.i.c.mzML --resolution=30000 --smoothwidth=2.5 --intwidth=2 --area=1 --numberofpeaks=10


### this file are not going to be picked since missing profile spectrum CV term.
### the output might be larger than input because of lack of compression.
$PROJECTDIR/findMFHCSout/PeakPicker --in $PROJECTDIR/submodule/findMFData/dil_6_2.mzML --out $PROJECTDIR/tmp/dil_6_2.a.c.mzML --resolution=30000 --smoothwidth=2.5 --intwidth=2 --area=0
ls -l $PROJECTDIR/submodule/findMFData/dil_6_2.mzML
ls -l $PROJECTDIR/tmp/dil_6_2.a.c.mzML
# area
$PROJECTDIR/findMFHCSout/PeakPicker --in $PROJECTDIR/submodule/findMFData/dil_6_2.mzML --out $PROJECTDIR/tmp/dil_6_2.i.c.mzML --resolution=30000 --smoothwidth=2.5 --intwidth=2 --area=1
ls -l $PROJECTDIR/submodule/findMFData/dil_6_2.mzML
ls -l $PROJECTDIR/tmp/dil_6_2.a.c.mzML

