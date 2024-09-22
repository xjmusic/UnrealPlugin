TAG="v1.9.6"
REPOSITORY_PATH="xj/"
CMAKE_PATH="xj/engine/"

git clone https://github.com/xjmusic/xjmusic.git "$REPOSITORY_PATH"

cd "$CMAKE_PATH"

git checkout tags/"$TAG"

mkdir -p build
cd build
cmake ..

cmake --build . --config Release --target xjmusic
