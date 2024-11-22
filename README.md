# -环形缓冲区（Ring-Buffer）

一个为嵌入式系统设计的简单环形缓冲区（循环缓冲区）。

为嵌入式系统精简优化的环形队列，源代码仅 80 行，实现简洁可靠。

在 examples/simple.c 中提供了一个示例。

内存缓冲区的大小必须是2的幂，且环形缓冲区最多可以包含 buf_size-1 个字节。

通过 ring_buffer_init(&ring_buffer, buff, sizeof(buff)) 函数创建一个新的环形缓冲区：

char buff[64];
ring_buffer_t ring_buffer;
ring_buffer_init(&ring_buffer, buff, sizeof(buff));
在这个例子中，缓冲区大小为64字节，环形缓冲区可以包含63字节。

该模块提供了以下用于访问环形缓冲区的函数（文档可以在 ringbuffer.h 中找到）：

void ring_buffer_queue(ring_buffer_t *buffer, char data); // 将数据入队到环形缓冲区
void ring_buffer_queue_arr(ring_buffer_t *buffer, const char *data, ring_buffer_size_t size); // 将数据数组入队到环形缓冲区
uint8_t ring_buffer_dequeue(ring_buffer_t *buffer, char *data); // 从环形缓冲区出队一个数据
ring_buffer_size_t ring_buffer_dequeue_arr(ring_buffer_t *buffer, char *data, ring_buffer_size_t len); // 从环形缓冲区出队多个数据到数组
uint8_t ring_buffer_peek(ring_buffer_t *buffer, char *data, ring_buffer_size_t index); // 查看环形缓冲区中指定索引的数据
uint8_t ring_buffer_is_empty(ring_buffer_t *buffer); // 返回环形缓冲区是否为空
uint8_t ring_buffer_is_full(ring_buffer_t *buffer); // 返回环形缓冲区是否已满
ring_buffer_size_t ring_buffer_num_items(ring_buffer_t *buffer); // 返回环形缓冲区中的项目数
