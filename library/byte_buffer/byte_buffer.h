/**
 * @file byte_buffer.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-22
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <rtthread.h>
#include <stdint.h>
#ifndef __BYTE_BUFFER_H__
#define __BYTE_BUFFER_H__

typedef struct
{
    /**
     * @brief 字节缓存
     *
     */
    int8_t *_buffer;
    /**
     * @brief 当前位置
     *
     */
    size_t _position;
    size_t _limit;
    /**
     * @brief 自动扩容后的大小
     *
     */
    size_t _alloc_capacity;
    /**
     * @brief 初始化容量
     *
     */
    const size_t _capacity;
} Buffer, *BufferPtr;

typedef struct
{
    Buffer (*allocate)(const size_t capacity);
    Buffer (*allocate_default)();
    void (*put_byte)(const BufferPtr buffer_ptr, const int8_t byte);
    int8_t (*get_byte)(const BufferPtr buffer_ptr);
    int8_t (*get_position_byte)(const BufferPtr buffer_ptr, size_t position);
    void (*put_bytes)(const BufferPtr buffer_ptr, const int8_t *bytes, const size_t len);
    int8_t *(*get_bytes)(const BufferPtr buffer_ptr, const size_t size);
    int8_t *(*get_range_bytes)(const BufferPtr buffer_ptr, const size_t start, const size_t end);
    void (*clear)(const BufferPtr buffer_ptr);
    void (*reset)(const BufferPtr buffer_ptr);
    void (*drop_memery)(const BufferPtr buffer_ptr);
    char *(*ref_string)(const BufferPtr buffer);
} _ByteBuffer;

extern const _ByteBuffer ByteBuffer;

#endif