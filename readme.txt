gcc xpy.c -o xpy
创建一个test.ly文件
输入print 123 abc 12.34 ddfasfs 12.34.56 一行
./xpy test.ly会生成ly.c 和ly。ly.c为自动生成的打印函数的程序