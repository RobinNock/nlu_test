#include "util.h"

// aes cbc pccs5加密*************************************************************************************************************
static struct AES_ctx ctx;
// pkcs5 填充
static uint8_t *pkcs5(const uint8_t *buf, size_t *len)
{
    uint32_t block_len = *len / AES_BLOCKLEN + 1;
    uint32_t padding_value = AES_BLOCKLEN - *len % AES_BLOCKLEN;
    uint8_t *new_buf = (uint8_t *)rt_malloc(block_len * AES_BLOCKLEN);
    new_buf = (uint8_t *)rt_memset(new_buf, padding_value, block_len * AES_BLOCKLEN);
    new_buf = (uint8_t *)rt_memcpy(new_buf, buf, *len);
    *len = block_len * AES_BLOCKLEN;
    return new_buf;
}
void aes_128_cbc_init()
{
    /* Initialize context */
    AES_init_ctx_iv(&ctx, KEY, IV);
}
/**
 * @brief 对传入字节序进行aes_128_cbc pkcs5 加密
 * @param buf 要加密的字节序
 * @param len 字节序长度
 * @return uint8_t*  返回以'\\0'结尾的加密后的字符串指针
 */
char *aes_128_cbc_encrypt(const uint8_t *buf, const size_t len)
{
    aes_128_cbc_init();
    size_t length = len;
    uint8_t *new_buf = pkcs5(buf, &length);
    AES_CBC_encrypt_buffer(&ctx, new_buf, length);
    new_buf = (uint8_t *)rt_realloc(new_buf, length + 1);
    new_buf[length] = '\0';
    return new_buf;
}
// hmac256*******************************************************************************************************************************
//生成固定32字节签名
/**
 * @brief 对传入的字节序计算sha256签名
 *
 * @param data 要签名的字节序
 * @param len  字节序长度
 * @return uint8_t* 返回以'\\0'结尾的签名hex字符串
 */
char *hmac_sha256_encrypt(const uint8_t *data, const size_t len)
{
    uint8_t *buf = (uint8_t *)rt_malloc((HMAC_SHA256_DIGEST_LENGTH) * sizeof(uint8_t));
    Hmacsha256_enc(KEY, rt_strlen(KEY), data, len, buf);
    char *chs = to_hex(buf, HMAC_SHA256_DIGEST_LENGTH);
    rt_free(buf);
    return chs;
}

// base64********************************************************************************************************************************
char *base64_en(const char *buf, const size_t len)
{
    return base64_encode(buf, len);
}

// tohex****************************************************************************************************************
char *to_hex(const char *buf, const size_t len)
{
    char *chs = (char *)rt_malloc((len * 2 + 1) * sizeof(uint8_t));
    for (uint32_t i = 0; i < len; i++)
    {
        chs[2 * i] = HEX[(buf[i] >> 4) & 0xF];
        chs[2 * i + 1] = HEX[buf[i] & 0xF];
    }
    chs[HMAC_SHA256_DIGEST_LENGTH * 2] = '\0';
    return chs;
}
//*************************************************************************************************************
/**
 * @brief 获取自unix时间间隔秒数
 *
 * @return uint32_t
 */
time_t now_sec_from_unix_epoch()
{
    const time_t now_sec = time((time_t *)NULL) - 2 * 60;
    return now_sec;
}
/**
 * @brief
 *
 */

size_t uint_len(uint32_t value)
{
    size_t len = 0;
    do
    {
        len += 1;
        value /= 10;
    } while (value != 0);
    return len;
}