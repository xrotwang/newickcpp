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
