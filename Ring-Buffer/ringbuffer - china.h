#include <inttypes.h>
#include <stddef.h>
#include <assert.h>

/**
 * @file
 * 环形缓冲区模块的原型和结构体。
 *  @author LYJ <1035945832@qq.com>
 */

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#define RING_BUFFER_ASSERT(x) assert(x)

/**
 * 检查缓冲区大小是否为2的幂。
 * 由于设计原因，缓冲区只能包含 <tt> RING_BUFFER_SIZE-1 </tt> 个项目。
 * buffer_size 必须是2的幂。
 */
#define RING_BUFFER_IS_POWER_OF_TWO(buffer_size) ((buffer_size & (buffer_size - 1)) == 0)

/**
 * 用于保存大小和索引的类型。
 */
typedef size_t ring_buffer_size_t;

/**
 * 作为模运算符使用
 * 因为 <tt> a % b = (a & (b − 1)) </tt>
 * 其中 \c a 是缓冲区中的正索引，\c b 是（2的幂）缓冲区的大小。
 */
#define RING_BUFFER_MASK(rb) (rb->buffer_mask)

/**
 * 简化对 <tt>struct ring_buffer_t</tt> 的使用。
 */
typedef struct ring_buffer_t ring_buffer_t;

/**
 * 保存环形缓冲区的结构体。
 * 缓冲区包含一个缓冲数组以及环形缓冲区的元数据。
 */
struct ring_buffer_t {
  /** 缓冲内存。 */
  char *buffer;
  /** 缓冲掩码。 */
  ring_buffer_size_t buffer_mask;
  /** 尾索引。 */
  ring_buffer_size_t tail_index;
  /** 头索引。 */
  ring_buffer_size_t head_index;
};

/**
 * 初始化 <em>buffer</em> 指向的环形缓冲区。
 * 此函数也可以用来清空/重置缓冲区。
 * 结果缓冲区可以包含 <em>buf_size-1</em> 字节。
 * @param buffer 要初始化的环形缓冲区。
 * @param buf 为环形缓冲区分配的缓冲区。
 * @param buf_size 分配的环形缓冲区的大小。
 */
void ring_buffer_init(ring_buffer_t *buffer, char *buf, size_t buf_size);

/**
 * 向环形缓冲区添加一个字节。
 * @param buffer 应该放置数据的缓冲区。
 * @param data 要放置的字节。
 */
void ring_buffer_queue(ring_buffer_t *buffer, char data);

/**
 * 向环形缓冲区添加一个字节数组。
 * @param buffer 应该放置数据的缓冲区。
 * @param data 指向要放入队列的字节数组的指针。
 * @param size 数组的大小。
 */
void ring_buffer_queue_arr(ring_buffer_t *buffer, const char *data, ring_buffer_size_t size);

/**
 * 返回环形缓冲区中最旧的字节。
 * @param buffer 应该从中返回数据的缓冲区。
 * @param data 指向应该放置数据的位置的指针。
 * @return 如果返回了数据则为1；否则为0。
 */
uint8_t ring_buffer_dequeue(ring_buffer_t *buffer, char *data);

/**
 * 返回环形缓冲区中 <em>len</em> 个最旧的字节。
 * @param buffer 应该从中返回数据的缓冲区。
 * @param data 指向应该放置数据的数组的指针。
 * @param len 要返回的最大字节数。
 * @return 返回的字节数。
 */
ring_buffer_size_t ring_buffer_dequeue_arr(ring_buffer_t *buffer, char *data, ring_buffer_size_t len);

/**
 * 查看环形缓冲区，即返回一个元素而不移除它。
 * @param buffer 应该从中返回数据的缓冲区。
 * @param data 指向应该放置数据的位置的指针。
 * @param index 要查看的索引。
 * @return 如果返回了数据则为1；否则为0。
 */
uint8_t ring_buffer_peek(ring_buffer_t *buffer, char *data, ring_buffer_size_t index);

/**
 * 返回环形缓冲区是否为空。
 * @param buffer 应该返回其是否为空的缓冲区。
 * @return 如果为空则为1；否则为0。
 */
inline uint8_t ring_buffer_is_empty(ring_buffer_t *buffer) {
  return (buffer->head_index == buffer->tail_index);
}

/**
 * 返回环形缓冲区是否已满。
 * @param buffer 应该返回其是否已满的缓冲区。
 * @return 如果已满则为1；否则为0。
 */
inline uint8_t ring_buffer_is_full(ring_buffer_t *buffer) {
  return ((buffer->head_index - buffer->tail_index) & RING_BUFFER_MASK(buffer)) == RING_BUFFER_MASK(buffer);
}

/**
 * 返回环形缓冲区中的项目数。
 * @param buffer 应该返回项目数的缓冲区。
 *  @return 缓冲区中的项目数。
 */
inline ring_buffer_size_t ring_buffer_num_items(ring_buffer_t *buffer) {
  return ((buffer->head_index - buffer->tail_index) & RING_BUFFER_MASK(buffer));
}

#ifdef __cplusplus
}
#endif

#endif /* RINGBUFFER_H */
