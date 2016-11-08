# PJD-FSTEST #
- fstest是一套简化版的文件系统POSIX兼容性测试套件，它可以工作在FreeBSD, Solaris, Linux上用于测试UFS, ZFS, ext3, XFS and the NTFS-3G等文件系统。fstest目前有3601个回归测试用例，测试的系统调用覆盖chmod, chown, link, mkdir, mkfifo, open, rename, rmdir, symlink, truncate, unlink。

## 配置和使用 ##
- pjd-fstest官网：http://www.tuxera.com/community/posix-test-suite/

- 进入官网下载 pjd-fstest-20080816.tgz 这个稳定版压缩包。

![](http://i.imgur.com/Tsqtsp4.png)

- 使用命令：`tar -xvf pjd-fstest-20080816.tgz`解压安装该工具，建议安装在home目录下。

- 安装完成后，home目录下会多出一个  pjd-fstest-20080816 文件夹，`cd pjd-fstest-20080816`进入该文件夹。

- 运行命令：`vim tests/conf`　编辑conf文件。

- 修改OS的值，由于在linux系统下测试，应将该值改为Linux。

- 修改fs的值，fstest工具支持UFS、ZFS、ext3、ntfs-3g、xfs五种文件格式，这里采用标准的Linux系统ext3文件系统，应将该值改为ext3。修改完后如下图：

![](http://i.imgur.com/9GdGsAK.png)

- 使用 `make` 命令开始编译，编译完后就可以使用该工具了。

- 使用该工具要用到以下两条代码:

 1. `cd /path/to/file/system/you/want/to/test/`
 
 2. `prove -r /path/to/fstest`
 
　　　命令1中：/path/to/file/system/you/want/to/test/　指的是你想测试的文件系统的绝对路径。

　　　命令2中：/path/to/fstest　指的是pjd-fstest-20080816这个文件夹的绝对路径，这里-r是指递归。需要注意的是，执行该命令必须获取root权限，可以`sudo su`指令切换到root模式，再运行这条指令。
## 制作ext3文件系统格式的u盘 ##
- 准备一个空u盘（8G）

- 将u盘插入运行有Linux系统的电脑中。Linux系统下所有的东西都是以文件的形式保存的，u盘也不例外。一般来说u盘插入后，系统会自动为其设置一个挂载点，这里为了方便起见，手动为u盘设置一个挂载点。

- 用`fdisk -l`命令查看硬盘分区，如图所示，可以看到u盘所在的设备为：/dev/sdb1。

![](http://i.imgur.com/3FditPL.png)

- 使用命令：`sudo mkfs -t ext3 /dev/sdb1`更改u盘的文件系统格式。

- 使用命令：`sudo mkdir /mnt/usb` 在mnt文件夹下创建一个usb文件夹。

- 使用命令：`sudo mount -t ext3 /dev/sdb1 /mnt/usb`将u盘挂载到/mnt/usb位置。

- 挂载完毕后可以使用`mount -l | grep ext3`命令查看是否挂载成功。

## 测试 ##
- 运行代码：`cd /mnt/usb`和`prove -r /**/pjd-fstest-20080816`（-r参数后面的路径是pjd这个文件夹的绝对路径）。开始测试，测试结果如下：

![](http://i.imgur.com/1MZiFgl.png)

- 显示所有测试都成功。底下是一些参数：files、tests、wallclock secs。

- 如果测试失败，则会报错：

![](http://i.imgur.com/U56HRMU.png)

- 如图可知：chown系统调用不通过。

- posix标准定义的系统调用函数形式如下：

![](http://i.imgur.com/GxpoDuZ.png)

- 图中未列出的chflags函数是定义在FreeBSD系统下的修改文件标志的命令。

- 对单个系统调用函数的测试：

在root模式下，进入pjdfstest文件目录下，使用命令：`./fstest -u 0 系统调用 参数`

![](http://i.imgur.com/aYG0c5c.png)

其中：-u表示uid，U表示mode，g表示gid， 0表示超级用户，系统调用和参数形式已在上表列出。从结果可知测试是通过的，mkdir和rmdir两个系统调用与posix标准兼容。如果出错，则会返回-1。

![](http://i.imgur.com/CeqSafY.png)

上图是由于mkdir函数创建了一个已经存在的文件夹，所以报错。

对于chflags函数，测试结果如下：

![](http://i.imgur.com/teCsQUl.png)
 
说明linux系统是不支持chflags这个系统调用的。



