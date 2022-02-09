/**
 * @file byte_buffer.c
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-22
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "byte_buffer.h"

//扩容因子
const static float EXPANSION_FACTOR = 0.75f;
const static size_t DEFAULT_CAPACITY = 16;

static Buffer buffer_default_capacity();
/**
 * @brief
 *
 * @param _capacity
 * @return Buffer
 */
static Buffer buffer_with_capacity(const size_t capacity);
/**
 * @brief
 *
 * @param buffer_ptr
 * @param byte
 */
static void buffer_put_byte(const BufferPtr buffer_ptr, const int8_t byte);
/**
 * @brief
 *
 * @param buffer_ptr
 * @param byte
 * @param len
 */
static void buffer_put_bytes(const BufferPtr buffer_ptr, const int8_t *bytes, const size_t len);
/**
 * @brief 获取单个字节，不安全不会检查溢出
 *
 * @param buffer_ptr
 * @return int8_t
 */
static int8_t buffer_get_byte(const BufferPtr buffer_ptr);
static int8_t buffer_get_position_byte(const BufferPtr buffer_ptr, size_t position);

/**
 * @brief 获取指定位置长度bytes，不安全不会检查溢出
 *
 * @param buffer_ptr
 * @param start
 * @param size
 * @return int8_t*
 */
static int8_t *buffer_get_bytes(const BufferPtr buffer_ptr, const size_t size);
/**
 * @brief 返回start ~ end 范围内byte数组，闭区间
 *
 * @param buffer_ptr
 * @param start
 * @param end
 * @return int8_t*
 */
static int8_t *buffer_get_range_bytes(const BufferPtr buffer_ptr, const size_t start, const size_t end);
/**
 * @brief 检查是否需要resize
 *
 * @param buffer_ptr
 * @param put_len
 */
static void buffer_check_resize(const BufferPtr buffer_ptr, const size_t put_len);
/**
 * @brief 计算扩容大小 _alloc_capacity = _alloc_capacity * (EXPANSION_FACTOR + 1);
 *
 * @param now_size
 * @param _position
 * @param put_len
 * @return size_t
 */
static size_t buffer_calculate_resize(const size_t now_size, const size_t position, const size_t put_len);
/**
 * @brief 当_position >= new_capacity时，扩容_buffer,仅内部使用
 *
 * @param buffer_ptr
 */
static void buffer_resize(const BufferPtr buffer_ptr, const size_t size);
/**
 * @brief 将_buffer内容置为 0
 *
 * @param buffer_ptr
 */
static void buffer_clear(const BufferPtr buffer_ptr);
/**
 * @brief 将_buffer内容清除，同时_buffer恢复初始大小
 *
 * @param buffer_ptr
 */
static void buffer_reset(const BufferPtr buffer_ptr);
/**
 * @brief 释放_buffer
 *
 * @param buffer_ptr
 */
static void buffer_free_memory(const BufferPtr buffer_ptr);
/**
 * @brief 获取以'\0'结尾的字符串应用，所有权仍属于BufferPtr,请勿释放
 *
 * @param buffer_ptr
 * @return char*
 */
static char *buffer_ref_string(const BufferPtr buffer_ptr);

static Buffer buffer_default_capacity()
{
    return buffer_with_capacity(DEFAULT_CAPACITY);
}

static Buffer
buffer_with_capacity(const size_t capacity)
{
    int8_t *bucket = (int8_t *)rt_malloc(capacity * sizeof(int8_t));
    Buffer buffer = {
        ._buffer = bucket,
        ._position = 0,
        ._limit = 0,
        ._alloc_capacity = capacity,
        ._capacity = capacity,
    };
    return buffer;
}

static void buffer_put_byte(const BufferPtr buffer_ptr, const int8_t byte)
{
    buffer_check_resize(buffer_ptr, 1);
    buffer_ptr->_buffer[buffer_ptr->_limit++] = byte;
}

static void buffer_put_bytes(const BufferPtr buffer_ptr, const int8_t *bytes, const size_t len)
{
    buffer_check_resize(buffer_ptr, len);
    for (size_t i = 0; i < len; i++)
    {
        buffer_put_byte(buffer_ptr, bytes[i]);
    }
}

static int8_t buffer_get_byte(const BufferPtr buffer_ptr)
{
    if (buffer_ptr->_position < buffer_ptr->_limit)
    {
        return buffer_ptr->_buffer[buffer_ptr->_position++];
    }
    return RT_NULL;
}

static int8_t buffer_get_position_byte(const BufferPtr buffer_ptr, size_t position)
{
    if (position < buffer_ptr->_limit)
    {
        return buffer_ptr->_buffer[position];
    }
    return RT_NULL;
}

static int8_t *buffer_get_bytes(const BufferPtr buffer_ptr, const size_t size)
{
    if (buffer_ptr->_limit - buffer_ptr->_position >= size)
    {
        int8_t *buffer = (int8_t *)rt_malloc(size * sizeof(int8_t));
        for (size_t i = 0; i < size; i++)
        {
            buffer[i] = buffer_get_byte(buffer_ptr);
        }
        return buffer;
    }
    return RT_NULL;
}

static int8_t *buffer_get_range_bytes(const BufferPtr buffer_ptr, const size_t start, const size_t end)
{
    if (start >= end || end >= buffer_ptr->_limit)
    {
        return RT_NULL;
    }
    int8_t *buffer = (int8_t *)rt_malloc((end - start + 1) * sizeof(int8_t));
    for (size_t i = start; i <= end; i++)
    {
        buffer[i - start] = buffer_ptr->_buffer[i];
    }
    return buffer;
}

static void buffer_check_resize(const BufferPtr buffer_ptr, const size_t put_len)
{
    if (buffer_ptr->_alloc_capacity - buffer_ptr->_limit < put_len)
    {
        const size_t size = buffer_calculate_resize(buffer_ptr->_alloc_capacity, buffer_ptr->_limit, put_len);
        buffer_resize(buffer_ptr, size);
    }
}

static size_t buffer_calculate_resize(const size_t now_size, const size_t limit, const size_t put_len)
{
    size_t size = (size_t)(EXPANSION_FACTOR * now_size) + now_size;
    if (size - limit < put_len)
    {
        size = buffer_calculate_resize(size, limit, put_len);
    }
    return size;
}

static void buffer_resize(const BufferPtr buffer_ptr, const size_t size)
{
    buffer_ptr->_buffer = (int8_t *)rt_realloc(buffer_ptr->_buffer, size * sizeof(int8_t));
    buffer_ptr->_alloc_capacity = size;
}

static void buffer_clear(const BufferPtr buffer_ptr)
{
    buffer_ptr->_buffer = (int8_t *)rt_memset(buffer_ptr->_buffer, 0, buffer_ptr->_alloc_capacity * sizeof(int8_t));
    buffer_ptr->_limit = 0;
    buffer_ptr->_position = 0;
}

static void buffer_reset(const BufferPtr buffer_ptr)
{
    buffer_ptr->_alloc_capacity = buffer_ptr->_capacity;
    buffer_ptr->_buffer = (int8_t *)rt_realloc(buffer_ptr->_buffer, buffer_ptr->_capacity * sizeof(int8_t));
    buffer_clear(buffer_ptr);
}

static void buffer_free_memory(const BufferPtr buffer_ptr)
{
    rt_free(buffer_ptr->_buffer);
}

static char *buffer_ref_string(const BufferPtr buffer_ptr)
{
    buffer_put_byte(buffer_ptr, '\0');
    return (char *)(buffer_ptr->_buffer);
}

const _ByteBuffer ByteBuffer = {
    .allocate = buffer_with_capacity,
    .allocate_default = buffer_default_capacity,
    .put_byte = buffer_put_byte,
    .put_bytes = buffer_put_bytes,
    .get_byte = buffer_get_byte,
    .get_position_byte = buffer_get_position_byte,
    .get_range_bytes = buffer_get_range_bytes,
    .get_bytes = buffer_get_bytes,
    .clear = buffer_clear,
    .reset = buffer_reset,
    .drop_memery = buffer_free_memory,
    .ref_string = buffer_ref_string,
};
