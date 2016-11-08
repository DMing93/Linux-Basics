##cyclictest

###简介

cyclictest 是一个高精度的测试程序，Cyclictest 是 rt-tests 下的一个测试工具，也是rt-tests 下使用最广泛的测试工具，一般主要用来测试使用内核的延迟，从而判断内核的实时性。

cyclictest github: [https://github.com/LITMUS-RT/cyclictest](https://github.com/LITMUS-RT/cyclictest)


###原理简析

cyclictest源码存放在rt-tests/cyclictest/目录中。

1. 在cyclictest.c中创建线程，线程开始与timerthread()函数。
1. 在timerthread()函数中，先获取当前时间（存放于全局静态变量new中）和计算出线程间隙（interval）。这时可计算出下个周期的理论时间（存放到全局静态变量的next中）。
1. 等待一个周期。
1. 一个周期之后获取这时当前时间（存放于全局静态变量new中），这时next和new的差值就是延迟时间。

这里只是粗略的介绍原理，但实际代码中有很多精妙的设置和计算，我很不理解。暂不介绍。

###安装

- 首先拷贝cyclictest的Git仓库

	git clone git://git.kernel.org/pub/scm/linux/kernel/git/clrkwllms/rt-tests.git

- 进入cyclictest
	
	cd rt-tests

- 创建一个分支

	git branch test

- 转到test分支，之后我们做的步骤都不会对主分支有影响

	git checkout test

- 查看我们当前在哪个分支

	git branch

- 配置环境

	sudo apt-get install build-essential libnuma-dev

- 编译

	make

- 查看编译后的rt-tests 目录

	ls

- ls查看我们编译好的rt-tests仓库，我们可以看到make 之后生成了很多可执行文件（黄色的），这些都是一个个测试工具。主要介绍的是cyclictest。

![](http://i.imgur.com/fczfcFy.png)

###参数介绍(常用) 

通过<code>./cyclictest -h</code>可以看到所有参数定义。这里只列举常用参数。

- -b USEC USEC定义了一个最大延时值,当延迟大于USEC指定的值时，发送停止跟踪的。单位us.<div>b是一个实时抢占补丁的控制延时跟踪的选项。 在一个系统中跟踪意外的大的延时是很有用的。这个选项只在内核编译时，有如下的配置选项时有作用：<br>
    <code>CONFIG\_PREEMPT\_RT=y</code><br>
    <code>CONFIG\_WAKEUP\_TIMING=y</code><br>
    <code>CONFIG\_LATENCY\_TRACE=y</code><br>
    <code>CONFIG\_CRITICAL\_PREEMPT_TIMING=y</code><br>
    <code>CONFIG\_CRITICAL\_IRQSOFF_TIMING=y </code><br>
-b 选项的参数 。这个值用来和测试的实际值进行比较。一个测量的延时大于USEC给定的值，内核跟踪和cyclictest将停止。跟踪可以从 /proc/latency_trace mybox中读出(现不存在)</div>

- -c CLOCK 选择时钟。0 = CLOCK\_MONOTONIC (default) 1 = CLOCK\_REALTIME 

- -d DIST 线程间隔（interval）的大小（默认为500us）。当cyclictest同时使用-d和-t参数时并且多个线程被创建，那么线程间隔大小的值增加到线程间隔中。Interval(thread N) = Interval(thread N-1) + DIST

- -i INTV 线程的基本间隔（interval），单位为us（默认1000us）。是设置第一个线程的间隔。

- -h US 在执行完后再标准输出设备上画出延迟直方图，US为最大跟踪时间限制（单位us）。若在终端，第一列表示延迟时间（单位us）；第二列表示次数。

- -l Loops 循环次数，默认为0（无限循环）

- -q 使用-q 参数运行时不打印信息，只在退出时打印概要内容，结合-h HISTNUM参数会在退出时打印HISTNUM 行统计信息以及一个总的概要信息。

###EXAMPLE

#### cyclictest的简单使用及结果分析

如果只是简单的测试机子的性能：进入rt-tests目录，运行：

	sudo ./cyclictest -t1 -p 80 -n -i 10000 -l 10000

![](http://i.imgur.com/MVPYLt9.png)

输出结果含义:

- T: 0 序号为0的线程，该线程tid为26405
- P: 运行线程优先级为80
- C: 9397 计数器。线程的时间间隔每达到一次，计数器加1，我设置循环10000，既i的参数。
- I: 1000 时间间隔为1000微秒(us)
- Min: 最小延时(us)
- Act: 最近一次的延时(us)
- Avg：平均延时(us)
- Max： 最大延时(us)


所以我们当前的机器上最小延时为1，平均为1，最大的为 53

####较为详细的分析当前内核

进入rt-tests目录，运行：

	sudo cyclictest -p 90 - m -c 0 -i 200 -n -h 100 -q -l 1000000

![](http://i.imgur.com/ckgi510.png)
![](http://i.imgur.com/u0QKrBK.png)

- 上图两列很长的数据表示:我们指定histogram = 100也就是只记录了0us～99us的值，那么第一列表示延迟时间（单位us）；第二列表示出现次数。
- Total表示总共有999997次延迟时间小于100us，Histogram overflow表示总共有3次延迟时间大于100us。总共测试1000000次。Histogram Overlow at cycle number及下一行表示延迟时间大于100us的在第几次循环出现。
- Min Latencies表示最小延迟。Avg Latencies表示平均延迟。Max Latencies表示最大延时1136us。

这些直方图数据可用gnuplot显示。如图：

![](http://i.imgur.com/A1wZ1tH.png)

可参照：[利用gnuplot图形化显示cyclictest的数据](http://iwojima.elastos.org/2014/01/07/%E5%88%A9%E7%94%A8gnuplot%E5%9B%BE%E5%BD%A2%E5%8C%96%E6%98%BE%E7%A4%BAcyclictest%E7%9A%84%E6%95%B0%E6%8D%AE/)

参考：

- http://blog.csdn.net/longerzone/article/details/16897655
- https://rt.wiki.kernel.org/index.php/Cyclictest
- https://github.com/LITMUS-RT/cyclictest