/**
 * @file nlu_request.c
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-22
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "nlu_request.h"

inline static char *request_new_auth(const char *body)
{
    char *auth_data = (char *)rt_malloc((rt_strlen(body) + 40) * sizeof(char)); //分配内存，返回内存地址
    const time_t timestamp = now_sec_from_unix_epoch();  //获取当前时间
    sprintf(auth_data, "%s%d", body, timestamp);   //将body与当前时刻timestamp内数据打印同时储存在auth_data中
    const char *signature = hmac_sha256_encrypt(auth_data, strlen(auth_data));  //将auth_data数据进行hmac_SHA256加密后存储在signature中
    rt_free(auth_data);   //释放auth_data所占内存
    // Appkey={},Timestamp={},Signature={}
    // Appkey长度:(len(Appkey=)+(len(Appkey))) <= 20 < 30
    // Timestamp长度: <= 20 + len(Timestamp=)  < 40
    // Signature长度: == 32 + len(Signature=) < 50
    // time_t 为无符号长整型，最大2^64 - 1,长度20位
    char auth_header[120];
    // 构造头部数据，将密码Appkey、当前时刻值Timestamp、与加密后的数据内容signature封装到头部auth_header中
    sprintf(auth_header, "Appkey=%s,Timestamp=%ld,Signature=%s", APP_KEY, timestamp, signature);   
    rt_free(signature);  //释放signature所占内存，其内容已保存至auth_header中
    return rt_strdup(auth_header);  //复制字符串auth_header的数据，返回复制后的字符串地址
}

inline static char *request_new_body(const BodyPtr content_ptr)
{
    const cJSON *content_cjson = BodyManager.base.json.to_cjson(content_ptr); //组装body
    const char *content_json = cJSON_PrintUnformatted(content_cjson); //将json格式转换为字符串
    cJSON_Delete(content_cjson);//释放内存
    const char *encrypt = aes_128_cbc_encrypt(content_json, strlen(content_json)); //aes_128_cbc模式加密
    rt_free(content_json);//释放内存
    const char *base64 = base64_en(encrypt, strlen(encrypt));  //base64加密
    rt_free(encrypt);//释放内存
    const Data data = DataManager.new(base64);   //转换为Data结构体
    rt_free(base64);//释放内存
    const cJSON *data_cjson = DataManager.base.json.to_cjson(&data);//将json格式转换为cjson格式封装，用于传输
    DataManager.base.drop.drop_memery(&data);//释放data所占内存
    const char *data_json = cJSON_PrintUnformatted(data_cjson);//以一定的格式把结构转换为字符串输出，即将data_cjson的cjson格式转换为data_jscon的json格式输出
    cJSON_Delete(data_cjson);//释放内存
    return data_json;
}

static Session request_new_session(const BodyPtr content_ptr)
{
    struct webclient_session *client = webclient_session_create(1024);  //创建用于存放当前建立http连接部分信息的结构体
    char *body = RT_NULL;
    if (client != RT_NULL)
    {
        body = request_new_body(content_ptr);//已组装好数据内容，且已经过aes与base64加密，返回字符串
        const char *auth = request_new_auth(body);   //组装数据，返回字符串地址
        webclient_header_fields_add(client, "Authorization: %s\r\n", auth);  //添加封装内容：Authorization:(auth)
        rt_free(auth); //释放auth所占内存
        webclient_header_fields_add(client, "Content-Length: %d\r\n", strlen(body));  //添加长度内容：Content-Length: (数据内容body长度)
        webclient_header_fields_add(client, "Cache-Control: no-cache\r\n");  //添加头部内容：Cache-Control: no-cache
        webclient_header_fields_add(client, "Host: nlu.gree.com\r\n");       //添加头部内容：Host: nlu.gree.com
        webclient_header_fields_add(client, "Body-Type: application/json;charset=UTF-8\r\n");  //添加头部内容：Body-Type: application/json;charset=UTF-8
    }
    Session session = {
        .client = client,
        .body = body,
    };    //封装结构体session，将连接的Http信息数据内容返回处理
    return session;
}

static int request_finish_post(const SessionPtr session_ptr, const BufferPtr buffer_ptr)
{
    int resp_status = webclient_post(session_ptr->client, ADDRESS, session_ptr->body);   //第一个参数为封装的http请求信息，第二个参数ADDRES为请求的nlu网址，第三个参数为发送的数据内容
    char *buffer = (char *)rt_malloc(1024);  //分配内存空间
    do//循环接收响应数据直到数据的接收
    {
        int bytes_read = webclient_read(session_ptr->client, buffer, 1024);            
        if (bytes_read <= 0)
        {
            break;
        }
        ByteBuffer.put_bytes(buffer_ptr, buffer, bytes_read);
    } while (1);
    rt_free(buffer);  //释放buffer内存
    return resp_status;
}

static void request_close(const SessionPtr session_ptr)
{
    if (session_ptr->client)
    {
        webclient_close(session_ptr->client);
    }
    rt_free(session_ptr->body);
}

const _NluRequest NluRequest = {
    .new = request_new_session,
    .post = request_finish_post,
    .close = request_close,
};