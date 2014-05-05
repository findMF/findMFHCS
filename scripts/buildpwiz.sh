git clone https://github.com/wolski/pwiz.git pwiz
cd pwiz
tar xf libpwiz_src_3_0_6002.tgz
cd pwiz
cd autotools
autoreconf --force --install
cd ..
./autotools/configure --prefix=${pwd}
make
make install

