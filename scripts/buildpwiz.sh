loc=$(pwd)
echo ${loc}
sudo git clone https://github.com/wolski/pwiz.git pwiz
cd pwiz
sudo tar xf libpwiz_src_3_0_6002.tgz
cd pwiz
cd autotools
sudo autoreconf --force --install
cd ..
sudo ./autotools/configure --prefix=${loc}/pwiz
sudo make
sudo make install

