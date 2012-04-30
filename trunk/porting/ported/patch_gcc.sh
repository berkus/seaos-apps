wget http://ftp.gnu.org/gnu/gcc/gcc-4.7.0/gcc-4.7.0.tar.gz
tar xzf gcc-4.7.0.tar.gz
cd gcc-4.7.0
patch -p1 -i ../patches/gcc-4.7.0-seaos.patch
cp ../files/seaos.h gcc/config/
