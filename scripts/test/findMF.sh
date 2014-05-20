#unzip $PROJECTDIR/submodule/findMFData/dil_6_2.mzML.zip -d  $PROJECTDIR/submodule/findMFData/
$PROJECTDIR/findMFHCSout/findMF --mzscale=1.3 --resolution=65000 --minMass=200 --nrthreads=2 --minintensity=5. --in=$PROJECTDIR/submodule/findMFData/dil_6_2.mzML

