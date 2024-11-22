#include "ringbuffer.h"

/**
 * @file
 * 环形缓冲区功能的实现。
 * @author LYJ <1035945832@qq.com>
 */

// 初始化环形缓冲区
void ring_buffer_init(ring_buffer_t *buffer, char *buf, size_t buf_size) {
  // 断言：缓冲区大小必须是2的幂
  RING_BUFFER_ASSERT(RING_BUFFER_IS_POWER_OF_TWO(buf_size) == 1);
  buffer->buffer = buf; // 设置缓冲区指针
  buffer->buffer_mask = buf_size - 1; // 计算掩码，用于环形索引
  buffer->tail_index = 0; // 初始化尾索引
  buffer->head_index = 0; // 初始化头索引
}

// 将数据入队到环形缓冲区
void ring_buffer_queue(ring_buffer_t *buffer, char data) {
  // 如果缓冲区已满
  if(ring_buffer_is_full(buffer)) {
    // 将覆盖最旧的数据
    // 增加尾索引
    buffer->tail_index = ((buffer->tail_index + 1) & RING_BUFFER_MASK(buffer));
  }

  // 将数据放入缓冲区
  buffer->buffer[buffer->head_index] = data;
  buffer->head_index = ((buffer->head_index + 1) & RING_BUFFER_MASK(buffer));
}

// 将数据数组入队到环形缓冲区
void ring_buffer_queue_arr(ring_buffer_t *buffer, const char *data, ring_buffer_size_t size) {
  // 逐字节添加
  ring_buffer_size_t i;
  for(i = 0; i < size; i++) {
    ring_buffer_queue(buffer, data[i]);
  }
}

// 从环形缓冲区出队一个数据
uint8_t ring_buffer_dequeue(ring_buffer_t *buffer, char *data) {
  if(ring_buffer_is_empty(buffer)) {
    // 没有数据项
    return 0;
  }

  *data = buffer->buffer[buffer->tail_index]; // 获取数据
  buffer->tail_index = ((buffer->tail_index + 1) & RING_BUFFER_MASK(buffer)); // 增加尾索引
  return 1;
}

// 从环形缓冲区出队多个数据到数组
ring_buffer_size_t ring_buffer_dequeue_arr(ring_buffer_t *buffer, char *data, ring_buffer_size_t len) {
  if(ring_buffer_is_empty(buffer)) {
    // 没有数据项
    return 0;
  }

  char *data_ptr = data; // 数据指针
  ring_buffer_size_t cnt = 0; // 计数器
  // 循环出队直到数组填满或缓冲区为空
  while((cnt < len) && ring_buffer_dequeue(buffer, data_ptr)) {
    cnt++;
    data_ptr++;
  }
  return cnt;
}

// 查看环形缓冲区中指定索引的数据
uint8_t ring_buffer_peek(ring_buffer_t *buffer, char *data, ring_buffer_size_t index) {
  if(index >= ring_buffer_num_items(buffer)) {
    // 指定索引没有数据项
    return 0;
  }

  // 计算数据索引
  ring_buffer_size_t data_index = ((buffer->tail_index + index) & RING_BUFFER_MASK(buffer));
  *data = buffer->buffer[data_index]; // 获取数据
  return 1;
}

// 以下是环形缓冲区的外部内联函数声明
extern inline uint8_t ring_buffer_is_empty(ring_buffer_t *buffer);
extern inline uint8_t ring_buffer_is_full(ring_buffer_t *buffer);
extern inline ring_buffer_size_t ring_buffer_num_items(ring_buffer_t *buffer);
