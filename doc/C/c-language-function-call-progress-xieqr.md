# C 语言函数调用过程分析——以 swap 函数为例

>By [谢启荣](mailto:xieqr16@lzu.edu.cn)

## 一、预备知识

1. 关于函数调用过程中的栈

	栈是向下生长的，即从内存高地址向低地址延伸，也就是说栈底在上栈顶在下，函数运行过程中栈底地址（栈基址）和栈顶地址分别保存在两个寄存器中。

	假设函数 A 调用函数 B 那么调用过程如下：

	1. 将 A 的栈基址入栈，以保存之前的任务信息;
	2. 将 A 的栈顶地址作为 B 的栈基址;
	3. 在这个基址上开辟一块空间（一般使用 `sub` 指令），作为 B 的栈空间;
	4. 在 B 返回后将 B 的栈基址作为 A 的栈顶指针，同时从栈顶弹出栈基址（调用 B 前压入栈中），现在就恢复到 A 的运行中了。

2. 汇编指令
	
	`mov` 数据传送指令，格式：`MOV SRC,DEST`，把一个字节，字或双字从源操作数 SRC 传送至目的操作数 DEST 。其中 `mov -4(%ebp), %eax` 含义为把 %ebp - 4 内存地址处的内容放到 eax 中。

	`lea` 地址传送指令，格式：`LEA REG，MEM`，将源操作数的有效地址传送到通用寄存器。

## 二、C swap 程序分析

创建两个 swap 程序，swap1.c 和 swap2.c
```c
// swap1.c
#include <stdio.h>

void swap(int x, int y) 
{
	int tmp = x;
	x = y;
	y = tmp;
	printf("x = %d, y = %d\n", x, y);
}

int main()
{
	int a = 1, b = 2;
	swap(a, b);
	printf("a = %d, b = %d\n", a, b);
	return 0;
} 
```
```c
// swap2.c
#include <stdio.h>

void swap(int *x, int *y) 
{
	int tmp = *x;
	*x = *y;
	*y = tmp;
	printf("*x = %d, *y = %d\n", *x, *y);
}

int main()
{
	int a = 1, b = 2;
	swap(&a, &b);
	printf("a = %d, b = %d\n", a, b);
	return 0;
} 
```
很明显，最终 swap1.c 中 `main` 函数中 `a` 和 `b` 的值不能被交换，因为其 `swap` 函数中只是交换了形参的值，并没有影响到 `main` 函数栈中 `a` 和 `b` 的值。而 swap2.c 中 `main` 函数中 `a` 和 `b` 的值最终能被交换，因为其 `swap` 函数中形参是指针，并分别指向函数栈中 `a` 和 `b`。下面我们通过分析反汇编代码来验证这一切。

## 三、实验验证

实验环境：系统 Arch Linux x64，处理器 Intel Core i5-3317U，GCC 版本 6.2.1 20160830

编译源代码并反汇编

```bash
# 编译
gcc swap1.c -o swap1
gcc swap2.c -o swap2

# 反汇编
objdump -d swap1 > swap1.dump
objdump -d swap2 > swap2.dump
```

先来分析 swap1 的反汇编文件，左边的 0x4004f6 为指令的地址

```S
0000000000400530 <main>: 	#main 函数
  400530:	55                   	push   %rbp        #保存调用 main 前的栈基址，设置 main 的栈基址
  400531:	48 89 e5             	mov    %rsp,%rbp   #rbp 寄存器存放栈基址，rsp 存放栈顶指针
  400534:	48 83 ec 10          	sub    $0x10,%rsp  #开辟 main 栈空间，大小为 0x10 个字节，即 16 字节
  400538:	c7 45 fc 01 00 00 00 	movl   $0x1,-0x4(%rbp) #把 1 入栈，地址为 rbp 向下偏移 4 个字节，即 a=1
  40053f:	c7 45 f8 02 00 00 00 	movl   $0x2,-0x8(%rbp) #把 2 入栈，地址为 rbp 向下便宜 8 个字节，即 b=2
  400546:	8b 55 f8             	mov    -0x8(%rbp),%edx 	#把 2 的值放在 edx 寄存器中
  400549:	8b 45 fc             	mov    -0x4(%rbp),%eax 	#把 1 的值放在 eax 寄存器中
  40054c:	89 d6                	mov    %edx,%esi 		#把 2 的值放到 esi 中
  40054e:	89 c7                	mov    %eax,%edi 		#把 1 的值放到 edi 中
  400550:	e8 a1 ff ff ff       	callq  4004f6 <swap>    #调用那个 swap 函数
  400555:	8b 55 f8             	mov    -0x8(%rbp),%edx 		#
  400558:	8b 45 fc             	mov    -0x4(%rbp),%eax 		#从main函数栈中提取a 和 b
  40055b:	89 c6                	mov    %eax,%esi	   		#的值并打印输出
  40055d:	bf 14 06 40 00       	mov    $0x400614,%edi		#
  400562:	b8 00 00 00 00       	mov    $0x0,%eax			#
  400567:	e8 84 fe ff ff       	callq  4003f0 <printf@plt>	#
  40056c:	b8 00 00 00 00       	mov    $0x0,%eax
  400571:	c9                   	leaveq 
  400572:	c3                   	retq   
  400573:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  40057a:	00 00 00 
  40057d:	0f 1f 00             	nopl   (%rax)
```

```S
00000000004004f6 <swap>:
  4004f6:	55                   	push   %rbp		  #保存调用swap前的栈基址，设置swap的栈基址
  4004f7:	48 89 e5             	mov    %rsp,%rbp  #rbp 寄存器存放栈基址，rsp 存放栈顶指针
  4004fa:	48 83 ec 20          	sub    $0x20,%rsp #开辟 swap 栈空间，大小为 0x20 个字节，即 32 字节
  4004fe:	89 7d ec             	mov    %edi,-0x14(%rbp)	#把 edi 的值（1）放到 rbp 向下偏移 20 字节处
  400501:	89 75 e8             	mov    %esi,-0x18(%rbp) #把 esi 的值（2）放到 rbp 向下偏移 24 字节处
  400504:	8b 45 ec             	mov    -0x14(%rbp),%eax #把 1 放入 eax
  400507:	89 45 fc             	mov    %eax,-0x4(%rbp)  #把 1 放到 rbp 向下4个字节处，即 tmp=x
  40050a:	8b 45 e8             	mov    -0x18(%rbp),%eax #把 2 放到 eax 中
  40050d:	89 45 ec             	mov    %eax,-0x14(%rbp) #把 2 放到rbp向下偏移 20字节处，即 x=y
  400510:	8b 45 fc             	mov    -0x4(%rbp),%eax  #把 1 放到 eax 中
  400513:	89 45 e8             	mov    %eax,-0x18(%rbp) #把 1 放到rbp向下24字节处，即y=tmp
  400516:	8b 55 e8             	mov    -0x18(%rbp),%edx    #把 1 放到edx中
  400519:	8b 45 ec             	mov    -0x14(%rbp),%eax    #把 2 放到eax中
  40051c:	89 c6                	mov    %eax,%esi 		   #把eax中的值（2）放到esi
  40051e:	bf 04 06 40 00       	mov    $0x400604,%edi      #把0x400604地址的值放到edi
  400523:	b8 00 00 00 00       	mov    $0x0,%eax		   #把0x0地址的值放到eax中
  400528:	e8 c3 fe ff ff       	callq  4003f0 <printf@plt> #调用printf
  40052d:	90                   	nop
  40052e:	c9                   	leaveq 
  40052f:	c3                   	retq
```
可以看出，在 `swap` 函数中互换只发生在 `swap` 函数栈中，并没有影响到 `main` 函数栈中存放 `a` 和 `b` 的内存，当 `main` 函数打印输出 `a` 和 `b` 的值的时候从 `main` 函数栈中取出的值并没有变化。

接下来分析 swap2 的反汇编代码：

```S
0000000000400544 <main>: 	#main函数
  400544:	55                   	push   %rbp
  400545:	48 89 e5             	mov    %rsp,%rbp
  400548:	48 83 ec 10          	sub    $0x10,%rsp
  40054c:	c7 45 fc 01 00 00 00 	movl   $0x1,-0x4(%rbp) 	#a = 1
  400553:	c7 45 f8 02 00 00 00 	movl   $0x2,-0x8(%rbp) 	#b = 2，和swap1中一样
  40055a:	48 8d 55 f8          	lea    -0x8(%rbp),%rdx 	#把 b 的地址放到rdx中
  40055e:	48 8d 45 fc          	lea    -0x4(%rbp),%rax 	#把 a 的地址放到rax中
  400562:	48 89 d6             	mov    %rdx,%rsi 		#把 b 的地址放到rsi中
  400565:	48 89 c7             	mov    %rax,%rdi 		#把 a 的地址放到rdi中
  400568:	e8 89 ff ff ff       	callq  4004f6 <swap> 	#调用 swap
  40056d:	8b 55 f8             	mov    -0x8(%rbp),%edx  	#
  400570:	8b 45 fc             	mov    -0x4(%rbp),%eax  	#从main函数栈中获取 a，b 
  400573:	89 c6                	mov    %eax,%esi			#的值此时内存中相应区域的
  400575:	bf 26 06 40 00       	mov    $0x400626,%edi		#内容已经发生变化，a 和 b
  40057a:	b8 00 00 00 00       	mov    $0x0,%eax			#的值已互换
  40057f:	e8 6c fe ff ff       	callq  4003f0 <printf@plt>	#
  400584:	b8 00 00 00 00       	mov    $0x0,%eax
  400589:	c9                   	leaveq 
  40058a:	c3                   	retq   
  40058b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
```

```S
00000000004004f6 <swap>:	#swap 函数
  4004f6:	55                   	push   %rbp
  4004f7:	48 89 e5             	mov    %rsp,%rbp
  4004fa:	48 83 ec 20          	sub    $0x20,%rsp 
  4004fe:	48 89 7d e8          	mov    %rdi,-0x18(%rbp) #将rdi中的值（即 a 的地址）放到rbp向下24字节处
  400502:	48 89 75 e0          	mov    %rsi,-0x20(%rbp) #将rsi中的值（即 b 的地址）放到rbp向下32字节处
  400506:	48 8b 45 e8          	mov    -0x18(%rbp),%rax #将 a 的地址放到 rax
  40050a:	8b 00                	mov    (%rax),%eax		#将 a 的地址所指向内存的内容（即a的值1）放到eax
  40050c:	89 45 fc             	mov    %eax,-0x4(%rbp)	#将 1 放到 rbp 向下偏移4个字节处
  40050f:	48 8b 45 e0          	mov    -0x20(%rbp),%rax	#将 b 的地址放到rax
  400513:	8b 10                	mov    (%rax),%edx		#将 b 的地址所指向内存的内容（即b的值2）放到edx
  400515:	48 8b 45 e8          	mov    -0x18(%rbp),%rax	#将 a 的地址放到rax
  400519:	89 10                	mov    %edx,(%rax)		#将 2 放到a的地址指向的内存区域
  40051b:	48 8b 45 e0          	mov    -0x20(%rbp),%rax	#将 b 的地址放到rax
  40051f:	8b 55 fc             	mov    -0x4(%rbp),%edx	#将 1 放到edx
  400522:	89 10                	mov    %edx,(%rax)		#将 1 放到 b 的地址指向的内存区域
  400524:	48 8b 45 e0          	mov    -0x20(%rbp),%rax	
  400528:	8b 10                	mov    (%rax),%edx		
  40052a:	48 8b 45 e8          	mov    -0x18(%rbp),%rax	
  40052e:	8b 00                	mov    (%rax),%eax 		
  400530:	89 c6                	mov    %eax,%esi
  400532:	bf 14 06 40 00       	mov    $0x400614,%edi
  400537:	b8 00 00 00 00       	mov    $0x0,%eax
  40053c:	e8 af fe ff ff       	callq  4003f0 <printf@plt>
  400541:	90                   	nop
  400542:	c9                   	leaveq 
  400543:	c3                   	retq   
```

可以看出，传入 `swap` 函数中的是 `a` 和 `b` 的内存地址，而在 `swap` 中交换的也是 `a` 和 `b` 相应内存中内容，所以当 `main` 函数中再去访问 `a` 和 `b` 的内存区域的时候其内容已经发生了变化，而 `swap` 函数栈中的内容并没有发生变化。

## 参考资料

- 《Linux 内核完全剖析——基于0.12内核》
- [c函数调用过程原理及函数栈帧分析](http://blog.csdn.net/zsy2020314/article/details/9429707)
- [x86汇编指令详解](http://blog.csdn.net/bekilledlzy/article/details/1765802)