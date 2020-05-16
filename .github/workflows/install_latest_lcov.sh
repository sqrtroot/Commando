#!/bin/sh
sudo apt install libjson-perl libperlio-gzip-perl 
directory=`mktemp /tmp/LCOV-XXXXXX -d`
git clone https://github.com/linux-test-project/lcov.git $directory 
cd "$directory" || exit 1
sudo make install