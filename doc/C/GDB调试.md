# GDB调试 #
- GDB是一个由GNU开源组织发布的、UNIX/Linux操作系统下的、基于命令行的、功能强大的程序调试工具。GDB中的命令很多，但我们只需掌握其中十个左右的命令，就大致可以完成日常的基本的程序调试工作。
- ![](http://i.imgur.com/VNdujBr.png)
- ![](http://i.imgur.com/3NFCsSU.png)


- 先给出一个小程序，用c语言写的，将此代码复制保存到文件 gdb-sample.c 中，然后切换到此目录，用GCC编译。

`#include <stdio.h>`

`int nGlobalVar = 0;`

`int tempFunction(int a, int b)`

`{`

        printf("tempFunction is called, a = %d, b = %d /n", a, b);
        return (a + b);
`}`

`int main()`

`{`

        int n;
        n = 1;
        n++;
        n--;

        nGlobalVar += 100;
        nGlobalVar -= 12;

        printf("n = %d, nGlobalVar = %d /n", n, nGlobalVar);

        n = tempFunction(1, 2);
        printf("n = %d", n);

        return 0;
`}`


- 使用命令：　`gcc -g gdb-sample.c -o gdb-sample `　编译程序。

- 在上述命令中，使用 -o 参数指定了编译生成的可执行文件名为 gdb-sample，使用参数 -g 表示将源代码信息编译到可执行文件中。

- 键入："gdb"命令启动GDB。

- ![](http://i.imgur.com/9z01c50.png)

- 使用“file”命令载入被调试程序 gdb-sample。

- ![](http://i.imgur.com/EOAYcDx.png)

- 使用“r”命令执行（Run）被调试文件，因为尚未设置任何断点，将直接执行到程序结束：

- ![](http://i.imgur.com/XqcVaCK.png)

- 下面使用“b”命令在 main 函数开头设置一个断点（Breakpoint）：

- ![](http://i.imgur.com/1MYtXB2.png)

- 上面最后一行提示已经成功设置断点，并给出了该断点信息：在源文件 gdb-sample.c 第19行处设置断点；这是本程序的第一个断点（序号为1）；断点处的代码地址为 0x804835c（此值仅在本次调试过程中有效）。回过头去看源代码，第19行中的代码为“n = 1”，恰好是 main 函数中的第一个可执行语句（前面的“int n;”为变量定义语句，并非可执行语句）。

- 再次使用“r”命令执行（Run）被调试程序：

- ![](http://i.imgur.com/SSZngl7.png)

- 程序中断在gdb-sample.c第19行处，即main函数的第一个可执行语句处。上面最后一行信息为：下一条将要执行的源代码为“n = 1;”，它是源代码文件gdb-sample.c中的第19行。

- 下面使用“s”命令（Step）执行下一行代码（即第19行“n = 1;”）：

- ![](http://i.imgur.com/62vTfvM.png)

- 上面的信息表示已经执行完“n = 1;”，并显示下一条要执行的代码为第20行的“n++;”。既然已经执行了“n = 1;”，即给变量 n 赋值为 1，那我们用“p”命令（Print）看一下变量 n 的值是不是 1 ：

- ![](http://i.imgur.com/TbSjrde.png)

- 结果果然是 1。（$1表示这是第一次使用“p”命令——再次执行“p n”将显示“$2 = 1”）

- 下面我们分别在第26行、tempFunction 函数开头各设置一个断点（分别使用命令“b 26”“b tempFunction”）：

- ![](http://i.imgur.com/EeeHnm0.png)

- 继续使用“c”命令（Continue）执行被调试程序，程序将中断在第二 个断点（26行），此时全局变量 nGlobalVar 的值应该是 88；再一次执行“c”命令，程序将中断于第三个断点（12行，tempFunction 函数开头处），此时tempFunction 函数的两个参数 a、b 的值应分别是 1 和 2：

- ![](http://i.imgur.com/ED2ioxp.png)

- 再一次执行“c”命令（Continue），因为后面再也没有其它断点，程序将一直执行到结束：

- ![](http://i.imgur.com/fpCRMku.png)

- 有时候需要看到编译器生成的汇编代码，以进行汇编级的调试或跟踪，又该如何操作呢？这就要用到display命令“display /i $pc”了（此命令前面已有详细解释）：

- ![](http://i.imgur.com/yvPlR3A.png)

- 此后程序再中断时，就可以显示出汇编代码了：

- ![](http://i.imgur.com/OXo0Wsk.png)

- “n = 1;”对应的汇编代码是“movl $0x1,0xfffffffc(%ebp)”。并且以后程序每次中断都将显示下一条汇编指定（“si”命令用于执行一条汇编代码——区别于“s”执行一行C代码）：

- ![](http://i.imgur.com/HRx9bk9.png)

- 接下来我们试一下命令“ b *<函数名称> ”。为了更简明，有必要先删除目前所有断点（使用“d”命令——Delete breakpoint）：

- ![](http://i.imgur.com/We9hXC7.png)

- 当被询问是否删除所有断点时，输入“y”并按回车键即可。下面使用命令“b *main”在 main 函数的开头设置断点

- ![](http://i.imgur.com/2Sf6SNt.png)

- 此时可以使用“i r”命令显示寄存器中的当前值———“i r”即“Infomation Register”：

- ![](http://i.imgur.com/Qx15VaF.png)

- 最后一个要介绍的命令是“q”，退出（Quit）GDB调试环境：(gdb) q

    ` The program is running. Exit anyway? (y or n)`
- 其他备注：
 1. list命令
     - `list <linenum>` ，显示程序第linenum行周围的源程序(将显示当前文件以“行号”为中心的前后10行代码)。
     - `list <function>` ，显示函数名为function的函数的源程序。
     - `list`，接着显示后面10行源程序。
     - `list` - ，显示前面10行源程序。
     
 2. watch命令
     - 用来观察某个表达式（变量也是一种表达式）的值是否有变化了，如果有变化，马上停住程序。我们有下面的几种方法来设置观察点： `watch <expr>`：为表达式（变量）expr设置一个观察点,一量表达式值有变化时，马上停住程序。`rwatch <expr>`：当表达式（变量）expr被读时，停住程序。`awatch <expr>`：当表达式（变量）的值被读或被写时，停住程序。`info watchpoints`：列出当前所设置了的所有观察点。
      
 3. examine命令
     - 我们可以使用examine命令（缩写为x）来查看内存地址中的值(不常用，这里没罗列)。
     
 4. set命令
     - set命令用来修改参数
     
 5. jump命令
     - jump <linespec> 来指定下一条语句的运行点。jump最好在同一个函数中进行跳转以免出错。
     
 6. signal命令
     - 使用singal命令，可以产生一个信号量给被调试的程序，如中断信号“Ctrl+C”。
     
 7. return命令
     - 如果在函数中设置了调试断点，在断点后还有语句没有执行完，这时候我们可以使用return命令强制函数忽略还没有执行的语句并返回。
     
 8. call命令
     - call命令用于强制调用某函数，并显示函数的返回值。


