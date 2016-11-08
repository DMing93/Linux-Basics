##Bonnie++（一个用来测试文件系统性能的程序）
Bonnie++是一款文件系统的基准性能自动化测试工具，包括测试文件系统的读写能力、查找能力、创建新文件的能力，它通过一系列的简单测试来生成文件系统的性能参数。主要目的是为了找出系统的性能瓶颈，其名字来源于作者喜爱的歌手Bonnie Raitt。
###下载安装
[下载Bonnie++的源码](http://www.coker.com.au/bonnie++/)
   
 解压
	
tar zxvf bonnie++-1.03a.tgz
    
进入解压后目录
	
./config
	
make
	
make install  (root)

##使用说明
Bonnie++将长时间的执行磁盘文件读写操作（中间不断给出进程报告），然后生成一个小而精的报告。

我们先了解bonnie++的命令参数

![](http://i.imgur.com/BrZ4b5C.jpg)
	
	

###常用命令参数说明
	-d 起始路径
    目录名字；Bonnie将在该目录下创建测试文件并进行读写，即为需要测试的文件系统挂载的目录。bonnie不会对目录名字进行任何特殊的解释，只是按照原名字使用。
    -s size-in-MB
    测试文件的大小，以MB为单位。默认是100MB大小。文件大小一定要是数倍于可用内存的大小，否则操作系统将会把文件的大部分文件cache到内存中，从而导致bonnie执行的真正的I/O次数非常少。建议设置至少为可用内存大小的2倍（另一种说法为4倍）。
    -m 机器名，
    实际上我们可以认为是本次测试的方案名，可以随便定义。默认是本机的hostname。
	-r 内存大小
    指定内存大小，这样可以通过-s参数创建r*2大小的文件，通常用于缩短测试时间，但是需要注意这样由于内存的cache可能导致测试结果的不准确。
	-x 测试的次数
	-u 测试文件的属主和组，默认是执行bonnie++的当前用户和当前组。
	-g 测试文件的组，默认是执行bonnie++的当前用组。
    -b 在每次写文件时调用fsync()函数，对于测试邮件服务器或者数据库服务器这种通常需要同步操作的情况比较适合，而不使用该参数则比较适合测试copy文件或者编译等操作的效率。

###测试
我们的文件系统挂在/home目录下，因此把工作目录定为/home
	
	# bonnie++ -d /home  -s 4000 -u root

输出结果：
	
![](http://i.imgur.com/6RC8euM.jpg)
	
	
这个结果不是很直观，bonnie++提供了bon_csv2html工具：
	
![](http://i.imgur.com/jZfHLtf.jpg)

调用bon_csv2html，得到html文件：

![](http://i.imgur.com/7CIpZoN.jpg)

####分析说明
从Writing with putc()开始到Delete files in random order…结束，这是bonnie++作的12项测试。

这12项测试依次对应12项结果，而这12项结果又被分为了5大类。

分别是Sequential Output（写测试），Sequential Input（读测试），Random Seeks（读写测试），Sequential Create（顺序读写文件测试），
Random Create（随意读写文件测试）。

Sequential Output部分表示写文件的相关信息

Sequential Input部分表示读文件的相关信息

Per Chr表示以字符为单位读写文件

Block表示以block为单位读写文件

Rewrite表示修改并重写已经存在的文件的每一个block

每个结果中又包括了2项数值，一个是K字节数或者文件数，另一个是%CP，就是执行这项测试时CPU的平均占用率。

	Sequential Output下的 Per Char是用putc方式写，毫无疑问，因为cache的line总是大于1字节的，所以不停的骚扰CPU执行putc，看到cpu使用率是99%.写的速度是63MB/s。
	Sequential Output是按照block去写的，明显CPU使用率就下来了，速度也上去了，越是97MB/s。
	Sequential Input下的Per Char是指用getc的方式读文件，速度是52MB/s，CPU使用率是94%。Sequential Input下的block是指按照block去读文件，速度是96.5MB/s,CPU使用率是6%。



对于输出结果的评价，我们认为在相等CPU的占用率情况下，存取字节数越高表示该存储设备的吞吐量越大，自然性能也就越好。

###测试结果的含义

>>Sequential Output	
>
>1． Per Char
>
>就是Per-Character的含义。使用putc()函数进行循环写入，每次写入的字节很小，基本上可以放入任意一种I-Cache中，这种情况下的CPU消耗在处理putc()代码和分配磁盘文件空间上
>
>2． Block
>
>使用write(2)函数创建文件。这种情况下的CPU消耗只是在分配磁盘文件空间上。
>
>3． Rewrite
>
>使用read(2)函数读取文件，然后修改再用write(2)函数写回。由于文件的空间已经分配好，所以这种方式可以很有效地测试文件系统缓存和数据传输的速度。
>>Sequential Input
>
>1． Per Char
>
>使用getc()函数循环 读取文件内容
>
>2． Block
>
>使用read(2)函数循环读取文件内容，有效测试磁盘读取的效率。

>>Random Seek
>
>默认3个seeks进程作8000次的测试。用read(2)函数读取文件的block，同时有10%的操作是用write(2)函数将block修改以 后写回文件中。在这个测试中，如果内存容量大于创建的文件大小，那么将会出现比较高的数值，而这个数值可能并不能准确反映磁盘本身的I/O效率。

>>Sequential Create和Radom Create
>
>这两大类测试均是用创建，读取，删除大量的小文件来测试磁盘效率。文件名用7位数字和任意个数（0-12）的任意英文字母来组成。在Sequential部分，字母在数字之后，而Random部分则是字母在数字之前。创建文件的最小值和最大值等参数可以在bonnie++命令行中用-n参数来控制。


参考文献：

<http://blog.chinaunix.net/uid-24774106-id-3728780.html>

<http://blog.csdn.net/choice_jj/article/details/8026130>

<http://blog.csdn.net/kamus/article/details/186714>

	