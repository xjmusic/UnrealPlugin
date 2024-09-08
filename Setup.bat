@echo off

SET "TAG=v1.9.5"
SET "XJ_LIB_PATH=Plugins\XJMusicPlugin\xj\engine"

git submodule update --init --recursive

cd "%XJ_LIB_PATH%"

git checkout tags/%TAG%

if not exist "build" mkdir build
cd build
cmake ..

cmake --build . --config Release --target xjmusic