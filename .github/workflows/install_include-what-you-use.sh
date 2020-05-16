set -e
sudo apt install llvm-10-dev libclang-10-dev
git clone https://github.com/include-what-you-use/include-what-you-use.git /iwyu
cd /iwyu
git checkout clang_10
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/usr/lib/llvm-10/lib/cmake/
make -j "$(nproc)"
make install
