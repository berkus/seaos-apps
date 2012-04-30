cd binutils-2.22
./configure --host=i586-pc-seaos --prefix=/usr --disable-nls
make  $MTHREAD
make install DESTDIR="`pwd`/../../../data/" 
