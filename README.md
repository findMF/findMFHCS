[![Build Status](https://travis-ci.org/findMF/findMFHCS.svg?branch=master)](https://travis-ci.org/findMF/findMFHCS) [![Project Stats](https://www.ohloh.net/p/findMFHCS/widgets/project_thin_badge.gif)](https://www.ohloh.net/p/findMFHCS)

findMF - find molecular features
==
Methods for feature detection in 1D and 2 D qTOF MS data.

Command line tools shipped with this package:
- PeakPicker qtof - peak picking in qTOF data
- findMF - feature finding in LCMS qTOF data (also mzML/mzXML file containing many maps)
- conv2Image - generate Images, and optionally apply filtering to LCMS qTOF data
- filterSingleMap - chemical noise removal in LCMS maps (in mzML out mzML with chemical noise removed)
- filterQTView - allows to visualize intermediate processing steps

Read the very brief [Article](https://github.com/findMF/findMFHCS/blob/master/ArticleDescribingProject.pdf?raw=true)
 describing the design decisions and concepts of findMF.

For usage examples see [checkSubmodule.sh](https://github.com/findMF/findMFHCS/blob/master/scripts/test/checkSubmodule.sh)

To find out more run the tools with the --help option.

See also project [findMFBase](https://github.com/findMF/findMFBase) which is a header only library implementing a lot of functions required by this project.

For how to build this project please refer to the [.travis.yml](https://github.com/findMF/findMFHCS/blob/master/.travis.yml)  file in the root directory of the project and to the 
*.sh files in the [buildscripts](https://github.com/findMF/findMFHCS/tree/master/buildscripts) folder refered to from the .travis.yml file.



