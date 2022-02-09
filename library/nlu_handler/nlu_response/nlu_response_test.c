
#include "library/nlu_handler/nlu_handler.h"
#include "nlu_response.h"
#include <finsh.h>
#include <rtthread.h>

void test_nlu_response()
{

    const User user = UserManager.new("", "");
    const Device device = DeviceManager.new("48d890d762b0", "48d890d762b0", "10f03");
    const Body content = BodyManager.new(user, device, "今天几号");
    Session session = NluRequest.new(&content);
    Buffer buffer = ByteBuffer.allocate(1024 * 8);
    if (session.client != RT_NULL)
    {
        int res = NluRequest.post(&session, &buffer);
        if (res == 200)
        {
            rt_kprintf("Request Success\n");
            const char *data = ByteBuffer.ref_string(&buffer);
            const cJSON *cjson_parser = cJSON_Parse(data);

            const Result result = NluResponse.from_cjson(cjson_parser);

            if (result.status_ptr != RT_NULL)
            {
                printf("status_ptr:\n");
                printf("\tcode:%d\n", result.status_ptr->code);
                printf("\terr_type:%s\n", result.status_ptr->error_type);
            }
            else
            {
                printf("status is null\n");
            }

            if (result.semantic_ptr != RT_NULL)
            {
                printf("semantic_ptr:\n");
                printf("\tdomain:%s\n", result.semantic_ptr->domain);
                printf("\tintent:%s\n", result.semantic_ptr->intent);
                printf("\tskill:%s\n", result.semantic_ptr->skill);
                printf("\tsession_complete:%d\n", result.semantic_ptr->session_complete);
            }
            else
            {
                printf("semantic is null\n");
            }

            if (result.response_ptr != RT_NULL)
            {
                printf("response_ptr:\n");
                printf("\ttext:%s\n", result.response_ptr->text);
                printf("\thint:%s\n", result.response_ptr->hint);
            }
            else
            {
                printf("response is null\n");
            }

            if (result.list_ptr != RT_NULL)
            {
                printf("list_ptr:\n");
                while (ListManager.has_next(result.list_ptr))
                {
                    DomainTypeEnum type = semantic_domain_type(result.semantic_ptr->domain);
                    if (type == MUSIC)
                    {
                        MusicPtr music_ptr = (MusicPtr)ListManager.next(result.list_ptr);
                        printf("\talbum:%s\n", music_ptr->album);
                        printf("\timage:%s\n", music_ptr->image);
                        printf("\tsinger:%s\n", music_ptr->singer);
                        printf("\tsong:%s\n", music_ptr->song);
                        printf("\tsong_id:%s\n", music_ptr->song_id);
                        printf("\turl:%s\n", music_ptr->url);
                    }
                }
            }
            else
            {
                printf("list is null\n");
            }

            NluResponse.drop_memory(&result);

            cJSON_Delete(cjson_parser);
        }
    }
    ByteBuffer.drop_memery(&buffer);
    NluRequest.close(&session);
    BodyManager.base.drop.drop_memery(&content);
}
MSH_CMD_EXPORT(test_nlu_response, test nlu handler);
