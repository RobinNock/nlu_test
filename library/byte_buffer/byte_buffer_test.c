#include "byte_buffer.h"
#include <finsh.h>

void test_byte_buffer_put()
{
    printf("test_byte_buffer_put ============================================\n");
    Buffer buffer = ByteBuffer.allocate(12);
    printf("buffer._position:%d\n", buffer._position);
    printf("buffer._limit:%d\n", buffer._limit);
    printf("buffer._alloc_capacity:%d\n", buffer._alloc_capacity);
    ByteBuffer.put_byte(&buffer, 'A');
    ByteBuffer.put_byte(&buffer, 'B');
    ByteBuffer.put_byte(&buffer, 'C');
    ByteBuffer.put_byte(&buffer, 'D');
    ByteBuffer.put_byte(&buffer, 'E');
    ByteBuffer.put_byte(&buffer, 'F');
    ByteBuffer.put_byte(&buffer, 'G');
    ByteBuffer.put_byte(&buffer, 'H');
    ByteBuffer.put_byte(&buffer, 'Y');
    ByteBuffer.put_byte(&buffer, 'J');
    ByteBuffer.put_byte(&buffer, 'K');
    ByteBuffer.put_byte(&buffer, 'M');

    printf("buffer._position:%d\n", buffer._position);
    printf("buffer._limit:%d\n", buffer._limit);

    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));

    ByteBuffer.put_byte(&buffer, 'N');
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    ByteBuffer.put_byte(&buffer, 'P');
    ByteBuffer.put_byte(&buffer, 'Q');
    ByteBuffer.put_byte(&buffer, 'J');
    ByteBuffer.put_byte(&buffer, 'O');
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));

    printf("buffer._position:%d\n", buffer._position);
    printf("buffer._limit:%d\n", buffer._limit);
    printf("buffer._alloc_capacity:%d\n", buffer._alloc_capacity);

    ByteBuffer.drop_memery(&buffer);
    printf("end ============================================\n");
}

void test_byte_buffer_puts()
{
    printf("test_byte_buffer_puts ============================================\n");
    Buffer buffer = ByteBuffer.allocate(12);
    printf("buffer._position:%d\n", buffer._position);
    printf("buffer._limit:%d\n", buffer._limit);
    printf("buffer._alloc_capacity:%d\n", buffer._alloc_capacity);

    char ch[] = "HelloWorld!\0";
    ByteBuffer.put_bytes(&buffer, ch, strlen(ch));
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    char *ch1 = ByteBuffer.get_bytes(&buffer, 5);
    for (int i = 0; i < 5; i++)
    {
        printf("%c", ch1[i]);
    }
    rt_free(ch1);
    printf("\n");
    printf("buffer get:%c\n", ByteBuffer.get_byte(&buffer));
    char *ch2 = ByteBuffer.get_range_bytes(&buffer, 0, 10);
    for (int i = 0; i <= 10; i++)
    {
        printf("%c", ch2[i]);
    }
    rt_free(ch2);
    printf("\n");
    printf("buffer._position:%d\n", buffer._position);
    printf("buffer._limit:%d\n", buffer._limit);
    printf("buffer._alloc_capacity:%d\n", buffer._alloc_capacity);
    const char *str = ByteBuffer.ref_string(&buffer);
    printf("buffer to string:%s\n", str);
    rt_free(str);
    ByteBuffer.drop_memery(&buffer);
    printf("end ============================================\n");
}

void test_byte_buffer_get(const BufferPtr buffer_ptr)
{
}

void test_byte_buffer_gets(const BufferPtr buffer_ptr)
{
}

void test_byte_buffer()
{
    test_byte_buffer_put();
    test_byte_buffer_puts();
}

MSH_CMD_EXPORT(test_byte_buffer, test byte buffer);
