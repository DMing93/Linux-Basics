# Linux下写简单C程序、编译C程序

###工具、环境
- gcc：the GNU Compiler Collection，最初是为GNU操作系统写的编译器，经过不断发展，成为一种支持C，C++，Objective-C，Fortran，Java等多种语言的源代码编译器集合。

- 实验环境：fedora

###Linux下写C程序

- 在Linux环境下启动一个shell终端（设置为同时按下ctrl+alt+T）。

- 在终端输入vim hello.c命令（文件名可以自行修改）。如果之前创建过以hello命名的.c文件，则vim将打开该文件，如果没有，将会自动创建一个hello.c文件并打开。如下图所示：

![](http://i.imgur.com/ihgQLy3.png)

- 进入新创建的hello.c文件中，编辑代码（输入i开始编写），完成后按ESC退出编辑模式，输入：wq保存并退出。

![](http://i.imgur.com/h6DzGVZ.png)

###Linux下编译C程序
- 返回终端输入gcc hello.c命令，按回车键进行编译，结果如图所示，如果报错，根据提示在对应的程序中进行修改。

![](http://i.imgur.com/5MOCu71.png)

- 通过编译生成了名为a.out的可执行文件，输入./a.out命令就可以运行hello.c程序，在屏幕上打印出结果。如图所示

![](http://i.imgur.com/EAZMNLB.png)

###GCC常用命令

实质上，编译的过程是分为四个阶段进行：预处理、编译、汇编和链接。

- 预处理

---
	gcc -E hello.c -o hello.i 或 gcc -E hello.c

输出的hello.i文件存放着hello.c经预处理后的代码，后面的命令将预处理后的代码在屏幕上打印出来。gcc -E选项，可以输出预处理的结果。预处理过程读入源代码，检查包含预处理指令的语句和宏定义，进行初步的转换并产生新的源代码提供给编译器。

下面两张图图是预处理结果的开头和结尾的部分内容。

![](http://i.imgur.com/S7JK9O9.png)

![](http://i.imgur.com/gYWKjg9.png)

- 编译

---
	gcc -S hello.i -o hello.s
编译之前，C语言编译器会进行词法分析、语法分析，再将源代码翻译成中间语言，即汇编代码。gcc -S选项，可以看到程序编译期间生成的汇编代码。下图是程序编译期间生成的汇编代码。

![](http://i.imgur.com/lbFBEMD.png)

- 汇编

---
	gcc -C hello.s -o hello.o
汇编实际还是编译过程，将作为中间结果的汇编代码翻译成机器代码，即目标代码，它还不能运行。gcc -c可以查看这一中间结果。下图是机器代码以十六进制方式显示的部分内容，右侧是汇编代码。

![](http://i.imgur.com/im8BUeD.png)

- 链接

---
	gcc hello.o -o hello
gcc连接器将程序的目标文件和所需的所有附加的目标文件连接起来，最终生成可执行文件。附加的目标文件包括静态链接库和动态链接库。将上面生成的hello.o与C标准输入输出链接，最终生成程序hello。

###编译多个程序文件

程序往往是由多个源文件组成，形成了多个与之对应的编译单元，如果有一个由name1.c和name2.c两个源文件组成的程序，对它们进行编译，最终生成可执行程序name.c，命令如下：

---
	gcc name1.c name2.c -o name

###检错

---
	gcc -pedantic  illcode.c -o illcode
 -pedantic选项能够帮助程序员发现一些不符合C标准的代码，但实际只有C语言标准中要求进行编译器诊断的情况，才可能被gcc发现并提出警告。除-pedantic之外，gcc还有其他编译选项可产生有用的警告信息，比如-Wall。

---
	gcc -Wall illcode.c -o illcode

gcc给出的警告信息虽然从原则上不能算作错误，但可能成为导致错误的隐患。如果使用-Werror选项，那么gcc会在产生警告的地方停止编译，程序员可对此进行修改。

---
	gcc -Werror test.c -o test

###库文件链接

通常编写程序时会借助很多库函数的支持已完成相应的功能。从程序员的角度来看，函数库实际上是各种头文件(.h)和库文件(so、lib、dll)的集合。Linux下大多函数都默认将头文件放在/usr/include/目录下，而库文件放在/usr/lib目录下，但有时候我们要用的库不在这些目录下，可以通过下面的命令来进行查找：

---
	find pathname -options
例如在/usr/include中查找stat.h，输入的命令如下(文件的格式是f)：

---
	find /usr/include -type f -name stat.h
之后可以对其进行查看，了解头文件的内容。

####编译成可执行文件
首先编译hello.c为目标文件，需要执行

---
	gcc -c -I /usr/include hello.c -o hello.o
####链接
将所有目标文件链接成可执行文件

---
	gcc -L /usr/lib/x86_64-linux-gnu （-static） hello.o -o hello
Linux下的库文件分为两大类：动态链接库（常以.so结尾）和静态链接库（常以.a结尾）。使用静态库需要在命令中输入-static。

静态库链接时搜索路径顺序：

1.ld会去找gcc命令中的参数-L

2.再找gcc环境变量LIBRARY_PATH

3.再找内定目录/lib /usr/lib /usr/local/lib 

动态链接时搜索路径顺序：

1.编译目标代码时指定的动态库搜索路径

2.环境变量LD_LIBRARY _PATH指定的动态库搜索路径

3.配置文件/etc/ld.so.conf中指定的动态库搜索路径

4.默认的动态库搜索路径/lib

5.默认的动态库搜索路径/usr/lib
