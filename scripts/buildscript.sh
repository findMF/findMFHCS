sudo mkdir findMFHCSbuild
cd findMFHCSbuild
sudo cmake -DPwiz_INCLUDE_DIR=/home/travis/build/findMF/findMFHCS/pwiz/pwiz -DPwiz_LIBRARY=/home/travis/build/findMF/findMFHCS/pwiz/lib ..
sudo make 
#make test
