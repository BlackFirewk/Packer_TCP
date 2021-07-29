rm -rf ./hiPacker_soft/HiPacker/*
rm hiPacker_soft.tar.gz
cp  ../release/HiPacker  ./hiPacker_soft/HiPacker/
cp -rf ./config ./hiPacker_soft/HiPacker
cd ./hiPacker_soft/HiPacker/
linuxdeployqt HiPacker -appimage
cd ../../
tar -czvf hiPacker_soft.tar.gz hiPacker_soft/
