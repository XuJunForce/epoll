# EPOLL相关函数

# #include <sys/epoll.h><br>
// 创建epoll实例，通过一棵红黑树管理待检测集合 这里的int size可忽略<br>

# int epoll_create(int size);<br>
// 管理红黑树上的文件描述符(添加、修改、删除)<br>

# int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);<br>

```C++
typedef union epoll_data {
 	void        *ptr;
	int          fd;	// 通常情况下使用这个成员, 和epoll_ctl的第三个参数相同即可
	uint32_t     u32;
	uint64_t     u64;
} epoll_data_t;


struct epoll_event {
	uint32_t     events;      /* Epoll events */
	epoll_data_t data;        /* User data variable */
};

```
op:
EPOLL_CTL_ADD<br>
EPOLL_CTL_MOD<br>
EPOLL_CTL_DEL<br>

event:
EPOLLIN：读事件, 接收数据, 检测读缓冲区，如果有数据该文件描述符就绪<br>
EPOLLOUT：写事件, 发送数据, 检测写缓冲区，如果可写该文件描述符就绪<br>
EPOLLERR：异常事件<br>



// 检测epoll树中是否有就绪的文件描述符<br>
# int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);<br>
epfd：epoll_create() 函数的返回值, 通过这个参数找到epoll实例<br>
events：传出参数, 这是一个结构体数组的地址, 里边存储了已就绪的文件描述符的信息<br>
maxevents：修饰第二个参数, 结构体数组的容量（元素个数）<br>
timeout：如果检测的epoll实例中没有已就绪的文件描述符，该函数阻塞的时长, 单位ms 毫秒<br>

