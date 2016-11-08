# Linux ubuntu和debian的安装 #
## 步骤 ##
1. 下载镜像，制作U盘启动盘
2. 安装系统
3. 一些基本的配置

## Ubuntu系统的安装 ##

- 镜像可以上[ubuntu官网](https://www.ubuntu.com "ubuntu官网")下载，镜像有32位和64位，选择合适的镜像下载即可。
- 用刻录工具将镜像刻录到u盘中，制作成启动盘。这里是用Universal USB Installer。
![](http://i.imgur.com/p3qTTaC.png)
- 当把镜像文件文件全部写入U盘后，在BIOS里设置为U盘启动或启动快捷键选择u盘启动（进入BIOS后，在BOOT或Startup界面将Removable Devices设置为第一启动项，然后保存退出重启；在开机时按下启动快捷键，选择u盘启动，各品牌电脑启动热键如下）。
![](http://i.imgur.com/lWhy2CV.jpg)
- 进入安装界面后，选择好语言（这里建议选择英语，因为如果出错不会产生乱码），按照安装步骤一步一步安装，注意：在安装类型界面选择其他选项（图像界面的安装很友好，这里就不一一插图了）。
- 为系统合理分区（下面会具体介绍如何分区以及每个分区的作用）
- 分区完等待安装，安装全部完成后，进行个人信息设置。安装到此就全部完成了。拔掉u盘，待电脑重启就可以进入ubuntu系统了。
- 配置（git、vim等常用工具以及能完成内核编译的环境），用命令：sudo apt-get install + 工具   就可以完成相应工具的安装。
编译内核的工具如：build-essential  (基本的编程库（gcc, make等） kernel-package   (Debian 系统里生成 kernel-image 的一些配置文件和工具) libncurses5-dev  (meke menuconfig要调用的）libqt3-headers   (make xconfig要调用的）其他工具在升级过程中可以按提示安装。

## Debian系统的安装 ##
- [Debain官网](https://www.debian.org)下载好镜像、刻录、制成u盘启动盘后，开机进入安装界面（建议下载完整的镜像，官网上的DVD镜像只需DVD-1即可）。
- 在引导界面选择Graphical install（图形化安装），进入安装过程（界面友好，不一一插图）。
 ![](http://i.imgur.com/HBmdl0z.png)
- 选择语言，设置好个人信息
- 磁盘分区
- 根据需要安装软件包
- 配置grub时，直接安装到MBR上即可
- 安装完成后，拔掉启动U盘，直接点继续。
- 开机后进入Grub引导界面，正常进入Grub应该是这个样子
 ![](http://i.imgur.com/stzMoKC.jpg)
- 直接按回车进入Debian，等系统加载完如果出现下面的界面就说明启动成功了（这是纯净安装，还没有安装桌面环境）。
 ![](http://i.imgur.com/Jwdxy20.jpg)
- 如果前面已经安装了桌面环境下面的步骤可以跳过：
- 在login后输入root，password后输入设置的超级管理员密码，以超级管理员权限进入系统。
输入命令：


     vi /etc/apt/sources.list


- 进入vi编辑器编辑源镜像站点。


- 按o可以新建一行进行编辑操作，输入以下内容。


	deb http://mirrors.163.com/debian wheezy main non-free contrib

	deb http://mirrors.163.com/debian wheezy-proposed-updates main contrib non-free

    deb-src http://mirrors.163.com/debian wheezy main non-free contrib

    deb-src http://mirrors.163.com/debian wheezy-proposed-updates main contrib non-free

    deb http://mirrors.163.com/debian-security wheezy/updates main contrib non-free


    deb-src http://mirrors.163.com/debian-security wheezy/updates main contrib non-free

    deb http://http.us.debian.org/debian wheezy main contrib non-free

    deb http://non-us.debian.org/debian-non-US wheezy/non-US main 
contrib non-free
    deb http://security.debian.org wheezy/updates main contrib non-free
    
- 输入完成后按键盘左上角的Esc键退出编辑模式，使用方向键移动光标进行定位，删除所有带有CD-ROM字样的条目。


- 连续按两次d，即快速输入dd可以删除光标所在的当前行（需退出编辑模式才能生效，否则就是加了两个d），也可用Delete键逐一删除。
删除完成后输入以下内容可完成保存和退出，注意前面有个冒号：

	:wq
    

- 退出vi后，输入以下命令更新软件源：



    ` apt-get update`


- 更新完成后，会发现中间有一些菱形的字符，这是因为Linux命令行界面下无法加载中文字符造成的。


- 安装中文字体：



     `apt-get install ttf-wqy-*`


- 安装xfce4图形界面，当然你也可以安装kde4或者gnome等等桌面环境，根据个人喜好了，不同桌面的优劣可以自行百度对比。


    ` apt-get install xfce4 xfce4-terminal`
    

- 安装完成后，输入以下命令注销root用户：



     `exit`


- 如果没有返回到login提示，再次重复上面的命令即可。


- 成功注销root用户后，用自己设定的用户名和密码登录，进入后输入一下命令即可完成桌面环境的启动。


     `startx`
    

- 这下就能看见桌面环境了。


- 执行下列命令进行系统更新：


    ` apt-get update`
    
    
    ` apt-get upgrade`
- 这种安装方法有个不足之处，就是每次启动后必须手动打命令启动桌面环境。

## 合理分区及每个分区的作用 ##
- 主分区和逻辑分区。安装Linux系统过程中，交换分区无挂载点，/目录设置主分区，其余可全部采用逻辑分区。
- /swap　交换分区  　它主要是把主内存上暂时不用的数据存起来，在需要的时候再调进内存内，且作为swap使用的分区不用指定“mout point”（载入点）。大小最好为物理内存的２倍，少于这个容量系统无法进入休眠。
- /　根目录 　 整个系统最顶层的文件目录，所有目录挂在其下。
- /boot  　这个分区包含了操作系统的内核和在启动系统过程中所要用到的文件，建这个分区是有必要的，因为目前大多数的PC机要受到BIOS的限制,况且如果有了一个单独的/boot启动分区，即使主要的根分区出现了问题，计算机依然能够启动。建议常编译内核的多分一些空间给/boot。
- /home　 用户的主目录。下面是自己定义的用户名的文件夹。每个用户的设置文件，用户的桌面文件夹，还有用户的数据都放在这里。剩下的内存都分配到这里。
- /usr　存放系统和个人软件的地方。
- /var/log　系统日志记录分区，如果设立了这一单独的分区，这样即使系统的日志文件出现了问题，它们也不会影响到操作系统的主分区。
- /tmp　tmp分区用来存放临时文件。对于网络服务器来说是有必要的。这样即使程序运行时生成大量的临时文件，或者用户对系统进行了错误的操作，文件系统的其它部分仍然是安全的。因为文件系统的这一部分仍然还承受着读写操作，所以它通常会比其它的部分更快地发生问题。
- /opt　附加程序存放地方。
- /root　系统管理员 （超级用户），即根用户的主目录。
- /etc　系统的配置文件目录。密码文件、设置网卡信息、环境变量的设置等都在此目录中，许多网络配置文件也在其中。
- ．．．．．．

### 常用的分区
- 一般分/swap、/、/boot和/home四个区。拿我自己的电脑来说，硬盘内存是500G，物理内存是12G，分给/boot分区100G，/分区100G，/swap交换空间24G，剩下的都分给/home分区。







