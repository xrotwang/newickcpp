[![Build and test](https://github.com/xrotwang/newickcpp/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/xrotwang/newickcpp/actions/workflows/cmake-single-platform.yml)

# newick

```shell
$ newick binarise -s "(a,b,c,d)e" |  newick print-ascii
   ┌a
─e─┤
   │  ┌b
   └──┤
      │  ┌c
      └──┤
         └d
```

## Install

```shell
cmake -B build
cmake --build build
cd build
make
sudo make install
```
