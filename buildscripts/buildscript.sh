sudo mkdir findMFHCSbuild
cd findMFHCSbuild
sudo cmake -DFindMFBase_INCLUDE_DIR=/home/travis/build/findMF/findMFHCS/findMFBase/include -DPwiz_INCLUDE_DIR=/home/travis/build/findMF/findMFHCS/pwiz/pwiz -DPwiz_LIBRARY=/home/travis/build/findMF/findMFHCS/pwiz/lib ..
sudo make 
sudo make test

