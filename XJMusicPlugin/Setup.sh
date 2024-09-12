TAG="v1.9.5"
REPOSITORY_PATH="Plugins/XJMusicPlugin/xj/"
CMAKE_PATH="Plugins/XJMusicPlugin/xj/engine/"

git clone https://github.com/xjmusic/xjmusic.git "$REPOSITORY_PATH"

cd "$CMAKE_PATH"

git checkout tags/"$TAG"

mkdir -p build
cd build
cmake ..

cmake --build . --config Release --target xjmusic