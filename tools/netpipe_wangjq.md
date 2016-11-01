#<center>Netpipe工具手册</center>
* Netpipe简介
* Netpipe的安装及使用
* 结果分析
* 总结
* 其它工具
* 参考资料

##简介
　　Netpipe是一种网络流量测量工具。这个工具目前由Troy Benjegerdes正在维护;最新的稳定版本是Netpipe-3.7.2。Netpipt由两部分组织: 独立于协议的驱动程序和针对各种协议(tcp、sctp等)的通信部分。Netpipe采用用于单一大小数据包的ping一pong传输模式，这使得网络在一定时间内只能传输某一指定大小数据包而非其他的,从而保证得到的测量结果是单一数据包的传输时间。Netpipe的原理是续发送大小不断增长的数据包, 接收服务方的确认信息, 每个指定大小的数据包在一定时间内不断循环传输, 取其平均值, 计算该数据包的传输速度(吞吐量)。

##Netpipe的安装及使用
###下载安装包
下载地址：
<a href="http://bitspjoule.org/netpipe/code/">http://bitspjoule.org/netpipe/code/</a>
###准备环境：
　　两台能接入互联网的主机
###安装及使用(以tcp为例)
1. 进入Netpipe目录：<font color="red"># cd Netpipe-3.7.2</font>
2. 编译：<font color="red"># make tcp</font>
   (<i>编译可能出现警告现象，不用去管</i>)
3. 上一步完成之后，在当前目录下会出现一个可执行文件NPtcp
4. 如果当前主机是A的话，你还需要一台接入互联网的主机B，在主机B上同样执行以上三步，完成NPtcp在主机B上的安装。
5. 确认两台主机的ip地址，在我这儿，主机A(local_host)ip地址是：<br>192.168.2.223<br>
主机B(remote_host)的ip地址是：<br>192.168.2.240
6. 完成以上步骤就可以正式使用Netpipe了，分别在两台主机上运行一下两条指令：
	<br>  主机A（local_host）: ./NPtcp -h remote_host  -o  output.txt
    <br>  主机B（remote_host）:./NPtcp
7. 上面两条指令正确运行之后，我们会在主机A（local_host）看到如下画面：
<br>![](http://i.imgur.com/CMGyDFB.png)<br>

8. 程序运行一段时间后，摁Ctrl+c终止程序，会发现当前目录下会生成一个文件output.txt。
  <br>![](http://i.imgur.com/uYOI4YQ.png)<br>
##结果分析
　　Netpipe运行后会默认生成一个文件np.out，我们可以通过-o选项给文件重命名(我把它命名为output.txt)。这个文件内容可以分为三列：字节数，吞吐量(Mbps)，往返时间除以2。下面output.txt部分内容的截屏<br>
![](http://i.imgur.com/ZGkFyIb.png)
<br>
　　其中字节数和吞吐量可以做一个曲线图，可以形象直观的观测到字节数量与吞吐量时间的关系，在ubuntu下采用gnuplot画出的图形如下：<br>
![](http://i.imgur.com/xtJiHC2.png)<br>
　　上面这个图中，横坐标代表的是字节数，纵坐标代表的是吞吐量，可以从图中很清晰的看出来，随着发送字节数的增加，吞吐量越来越大，到最后会到达一个上限保持不变。

##总结
　　这儿只是基于tcp协议的Netpipe网络性能测试，还可以基于sctp协议，其使用方法和tcp的类似，具体可以阅读Netpipt-3.7.2/dox/README.

##其它工具
* <a href="https://en.wikipedia.org/wiki/Netperf">Netperf</a>
* <a href="https://en.wikipedia.org/wiki/Iperf">Iperf</a>



##参考资料
* <a href="http://bitspjoule.org/netpipe/">官方文档</a>
* ./Netpipe-3.7.2/dox/README
* Linux环境下基于TCP_IP协议的网络流量测量与分析