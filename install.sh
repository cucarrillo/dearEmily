#!/bin/sh

cd rain
./build
sudo mv de_rain /usr/bin/
cd ..

cd snow
./build
sudo mv de_snow /usr/bin/
cd ..

cd emoticon
./build
sudo mv de_emoticon /usr/bin/
cd ..

cd script
sudo cp dearem /usr/bin/
cd ..

echo "install complete"
