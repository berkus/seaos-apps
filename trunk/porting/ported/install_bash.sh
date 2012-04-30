cd bash-4.2
./configure --without-bash-malloc --without-curses --disable-job-control --host=i586-pc-seaos --prefix=/ --datarootdir=/usr/share
make $MTHREAD
make install DESTDIR="`pwd`/../../../data/" 
