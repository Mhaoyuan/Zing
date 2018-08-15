# Zing WebServer

Zing is a high performance HTTP WebServer uses the Reactor. Code normative and functional scalability are close to the industry level.The projec will be until feature have been completed. Hava a fun. Whu,Zing

![2018-08-15 13-56-56屏幕截图](/home/genius/Pictures/2018-08-15 13-56-56屏幕截图.png)

# Dev document

| Part   Ⅰ                                                     | Part  Ⅱ                                                      | Part Ⅲ   | Part  Ⅳ  | Part Ⅴ   | Part  Ⅵ  | Part Ⅶ   | Part Ⅷ   | Part Ⅸ   |
| ------------------------------------------------------------ | ------------------------------------------------------------ | -------- | -------- | -------- | -------- | -------- | -------- | -------- |
| [项目目的](https://github.com/Mhaoyuan/Zing/blob/master/%E9%A1%B9%E7%9B%AE%E7%9B%AE%E7%9A%84.md) | [并发模型](https://github.com/Mhaoyuan/Zing/blob/master/%E5%B9%B6%E5%8F%91%E6%A8%A1%E5%9E%8B.md) | 核心结构 | 整体结构 | 主要函数 | 遇到问题 | 测试改进 | 背景知识 | 实用教程 |



# Dev Environment

## Dev Tool

* 操作系统：Ubuntu 18.04
* 编译器： VScode + Vim
* 编译器：g++ 7.3.0 
* 集成环境：Clion

## Other

* 自动化构建：Travis CI

* 测压工具：WebBench

  

# Timeline

### now

* v1.0已经完成，本地测试通过。提交到GitHub上的代码会由Travis自动构建

  特性：

   * 添加TImer定时器，定时回调handle处理超时请求
      * 高效的小根堆结构
      * 惰性删除方式
   * 实现HTTP长连接传输数据
      * 非阻塞I/O
      * epoll边缘触发模式（ET）
   * 线程池操作及其同步互斥管理
      * 调度选项
         * 队列式FIFO
         * 加入优先级的优先队列（+）
   * 使用扎状态机解析HTTP协议，非简单字符串匹配方式解析请求

### Feature

* v2.0实现FastCGI

  
