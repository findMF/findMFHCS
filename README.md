[![Build Status](https://travis-ci.org/findMF/findMFHCS.svg?branch=master)](https://travis-ci.org/findMF/findMFHCS)

findMF - find molecular features
==
Methods for feature detection in 1D and 2 D qTOF MS data.

Command line tools shipped with this package:
- PeakPicker qtof - peak picking in qTOF data
- findMF - feature finding in LCMS qTOF data (also mzML/mzXML file containing many maps)
- conv2Image - generate Images, and optionally apply filtering to LCMS qTOF data
- filterSingleMap - chemical noise removal in LCMS maps (in mzML out mzML with chemical noise removed)

if build with QT:
- filterQTView - allows to visualize intermediate processing steps

see also : (https://github.com/findMF/findMFBase)


For how to build this project please refer to the .travis.yml  file in the root directory of the project and to the files
*.sh files in the buildscript folder refered to from the .travis.yml file.



