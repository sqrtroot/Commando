sudo bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
sudo apt install -y clang-tidy-10 clang-format-10
sudo ln -s $(which clang-tidy-10) /usr/local/sbin/clang-tidy
sudo ln -s $(which clang-format-10) /usr/local/sbin/clang-format
