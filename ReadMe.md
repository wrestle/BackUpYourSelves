# BackUpYourSelves Version 1.2 
一个用C语言写的, Windows下的备份软件
，考虑到效率不使用GUI而使用 Console命令行窗口

其中界面字符为英文，源代码注释为中文

，速度 `  < 0.11 (hour/100GB)` 在 7200转的 机械硬盘上，经过疲劳读写后测试的数据。
## 当前实现的功能有：

- 使用Win32的多线程实现来进行编写(`#include <process>`)
- 修改备份存储的路径
- 保存使用过的的备份路径
- 选择要备份的地方
- 使用多线程技术(10线程用于备份)


## TODO

- 可以选择不备份某些文件夹
- 盘符可视化操作，当前是输入完整路径
- 使用POSIX协议的多线程实现来进行编写(pthreads)
