set -e
sudo apt install llvm-8-dev libclang-8-dev
git clone https://github.com/include-what-you-use/include-what-you-use.git /iwyu
cd /iwyu
git checkout clang_8.0
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j "$(nproc)"
make install
