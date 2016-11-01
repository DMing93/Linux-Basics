### 时间戳测试
即使对于非实时系统, 同样也要知道它对于事件的处理在时间上的精度是怎么样子的. 了解它时间上的精度也有利于我们去排查其他应用在测试过程中遭遇到的一些时间上异常的延迟.
这项测试的机理很简单, 甚至不需要使用到其他专业的工具, 只需要简单地连续运行两次clock\_gettime(),来获取两次运行的时间差即可. clock\_gettime()返回的数据类型struct timespec提供了纳秒级的时间记录, 并且函数中提供了多种标识符, 用于支持多种不同情况下的记录时间.
|标识符|说明|
|--------|------------|
|CLOCK_REALTIME|实时系统时间|
|CLOCK_MONOTONIC|不带负跳数的实时系统时间|
|CLOCK_PROCESS_CPUTIME_ID|调用进程的CPU时间|
|CLOCK_THREAD_CPUTIME_ID|调用线程的CPU时间|

我们在测试时将执行程序的nice值设置为最大, 这样就不会有其他任务来中断或者抢占该线程. 只有一些不可避免的中断可能会导致两次调用之间的延迟, 列如上半部中断和一些硬件中断.

下面是测试使用的代码:

	
	#include <iostream>
	#include <time.h>
	#include <unistd.h>
	using namespace std;

	timespec diff(timespec start, timespec end);

	int main()
	{
        timespec time1, time2;
        timespec a[20000];
        nice(95);
        for(int i = 0; i < 20000; i++){
                clock_gettime(CLOCK_REALTIME, &time1);
                clock_gettime(CLOCK_REALTIME, &time2);
                a[i] = diff(time1, time2);
        }
        for(int i = 0; i < 30; i++){
                cout<<a[i].tv_nsec<<endl;
        }
        sleep(5);
        return 0;
	}
	timespec diff(timespec start, timespec end)
	{
        timespec tmp;
        if((end.tv_nsec - start.tv_nsec) < 0){
                tmp.tv_sec = end.tv_sec - start.tv_sec - 1;
                tmp.tv_nsec =  1000000000 + end.tv_nsec - start.tv_nsec;
        }else{
                tmp.tv_sec = end.tv_sec - start.tv_sec;
                tmp.tv_nsec = end.tv_nsec - start.tv_nsec;
        }
        return tmp;
	}



其中在执行20000次之后, 才开始将结果打印出来. 由于输出是标准输出, 简化输出只打印了30条记录, 后续可以根据实际需要, 可以打印到文本或者直接在终端上打印更多结果.

![](http://i.imgur.com/aHnIH6r.png)