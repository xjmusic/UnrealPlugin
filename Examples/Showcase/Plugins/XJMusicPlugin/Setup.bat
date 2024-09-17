@echo off

SET "TAG=v1.9.5"
SET "REPOSITRY_PATH=.\xj\"
SET "CMAKE_PATH=.\xj\engine\"

git clone https://github.com/xjmusic/xjmusic.git %REPOSITRY_PATH%

cd "%CMAKE_PATH%"

git checkout tags/%TAG%

if not exist "build" mkdir build
cd build
cmake ..

cmake --build . --config Release --target xjmusic