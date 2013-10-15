findMF - find molecular features
==
Methods for feature detection in 1D and 2 D qTOF MS data.

How to build:

git clone git@github.com:wolski/findmf.git

mkdir findmf-build

cd findmf-build

ccmake ../findmf


Structure:


- include/base - header only library depends on boost only.
- src/base - tests testing the classes and methods in base


- include/findmf - applications PeakPicker and FindMF 
- src/finmf - implementation and tests

ccmake will ask you to specify header and library locations of the following libraries:
- gtest http://code.google.com/p/googletest/
- glog http://code.google.com/p/google-glog/
- vigra http://hci.iwr.uni-heidelberg.de/vigra/
- pwiz http://proteowizard.sourceforge.net/
- soci http://soci.sourceforge.net/


