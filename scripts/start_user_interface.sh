#!bin/bash

cd ~/finroc
source scripts/setenv -p smart_home
sudo UserInterface &
fingui sources/cpp/projects/smart_home/etc/heatcontrol.gui --fullscreen --connect=heatcontrol:4444 &
cd -
