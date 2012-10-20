#!/bin/sh
#extension_dir=/usr/local/lib/php/extensions/no-debug-non-zts-20090626
#extension_dir=/usr/local/lib/php/extensions/debug-zts-20090626
extension_dir=/usr/local/lib/php/extensions

make clean
phpize
./configure --enable-decimal
make
sudo mkdir -p $extension_dir
sudo cp modules/decimal.so $extension_dir
