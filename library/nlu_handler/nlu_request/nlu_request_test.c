#include "nlu_request.h"
#include <finsh.h>
#include <rtthread.h>

void test_request_data()
{
    const User user = UserManager.new("", "");
    const Device device = DeviceManager.new("48d890d762b0", "48d890d762b0", "10f03");
    const Body content = BodyManager.new(user, device, "播放红莲华");
    Session session = NluRequest.new(&content);
    Buffer buffer = ByteBuffer.allocate(1024 * 8);
    if (session.client != RT_NULL)
    {
        int res = NluRequest.post(&session, &buffer);
        if (res == 200)
        {
            rt_kprintf("Request Success\n");
            const char *data = ByteBuffer.ref_string(&buffer);
            for (int i = 0; data[i] != '\0'; i++)
            {
                rt_kprintf("%c", data[i]);
            }
            rt_kprintf("\n");
        }
    }
    ByteBuffer.drop_memery(&buffer);
    NluRequest.close(&session);
    BodyManager.base.drop.drop_memery(&content);
}

MSH_CMD_EXPORT(test_request_data, test requrest data);