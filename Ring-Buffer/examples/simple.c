#include <assert.h>
#include <stdio.h>
#include "../ringbuffer.h"

int main(void) {
  int i, cnt;
  char tmp;
  char tmp_arr[50];
  
  // 创建并初始化环形缓冲区
  ring_buffer_t ring_buffer;
  char buf_arr[128];
  ring_buffer_init(&ring_buffer, buf_arr, sizeof(buf_arr));
  
  // 逐个添加元素到缓冲区
  for(i = 0; i < 100; i++) {
    ring_buffer_queue(&ring_buffer, i);
  }

  // 验证大小
  assert(ring_buffer_num_items(&ring_buffer) == 100);

  // 查看第三个元素
  cnt = ring_buffer_peek(&ring_buffer, &tmp, 3);
  // 断言有字节返回
  assert(cnt == 1);
  // 断言内容
  assert(tmp == 3);

  // 出队所有元素
  for(cnt = 0; ring_buffer_dequeue(&ring_buffer, &tmp) > 0; cnt++) {
    // 对buf进行处理...
    assert(tmp == cnt);
    printf("读取: %d\n", tmp);
  }
  printf("\n===============\n");

  // 添加数组
  ring_buffer_queue_arr(&ring_buffer, "Hello, Ring Buffer!", 20);

  // 缓冲区是否为空？
  assert(!ring_buffer_is_empty(&ring_buffer));

  // 出队所有元素
  while(ring_buffer_dequeue(&ring_buffer, &tmp) > 0) {
    // 打印内容
    printf("读取: %c\n", tmp);
  }
  
  // 添加新数组
  ring_buffer_queue_arr(&ring_buffer, "Hello again, Ring Buffer!", 26);
  
  // 分两步出队数组
  printf("读取:\n");
  cnt = ring_buffer_dequeue_arr(&ring_buffer, tmp_arr, 13);
  printf("%d\n", cnt);
  assert(cnt == 13);
  // 在打印前添加\0终止符
  tmp_arr[13] = '\0';
  printf("%s\n", tmp_arr);
  // 出队剩余部分
  cnt = ring_buffer_dequeue_arr(&ring_buffer, tmp_arr, 13);
  assert(cnt == 13);
  printf("%s", tmp_arr);
  

  printf("\n===============\n");

  // 超量填充缓冲区
  for(i = 0; i < 1000; i++) {
    ring_buffer_queue(&ring_buffer, (i % 127));
  }
  
  // 缓冲区是否已满？
  if(ring_buffer_is_full(&ring_buffer)) {
    cnt = ring_buffer_num_items(&ring_buffer);
    printf("缓冲区已满，包含 %d 字节\n", cnt);
  }
  
  // 出队所有元素
  while(ring_buffer_dequeue(&ring_buffer, &tmp) > 0) {
    // 打印内容
    printf("读取: 0x%02x\n", tmp);
  }
  
  return 0;
}
