[![Build and test](https://github.com/xrotwang/newickcpp/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/xrotwang/newickcpp/actions/workflows/cmake-single-platform.yml)

# newick

```shell
$ ./cmake-build-release/newick binarise -s "(a,b,c,d)e" |  ./cmake-build-release/newick print-ascii
   ┌a
─e─┤
   │  ┌b
   └──┤
      │  ┌c
      └──┤
         └d
```

