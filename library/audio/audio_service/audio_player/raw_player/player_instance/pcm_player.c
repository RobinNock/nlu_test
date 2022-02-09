#include <sys/cdefs.h>
//
// Created by 202106 on 2021/11/23.
//

#include "pcm_player.h"


static struct {
    rt_mp_t buffer_pool;
    rt_mq_t buffer_queue;
    rt_event_t play_event;
    OnPcmPlayerEventCallback play_event_callback;
    rt_mutex_t play_event_callback_lock;
    struct {
        int sample_rate;
        int channel;
        int sample_bits;
    } audio_info;
    struct {
        int volume;
        char *device_name;
        rt_device_t device_handler;
    } device_info;
} PcmPlayer = {
        .buffer_pool = NULL,
        .buffer_queue = NULL,
        .play_event = NULL,
        .play_event_callback = NULL,
        .play_event_callback_lock = NULL,
        .device_info = {
                .volume = PCM_DEFAULT_VOLUME,
                .device_name = PCM_PLAYER_DEVICE_HARDWARE_NAME,
                .device_handler = NULL,
        },
        .audio_info = {
                .sample_rate = PCM_DEFAULT_SAMPLE_RATE,
                .channel = PCM_DEFAULT_CHANNEL,
                .sample_bits = PCM_DEFAULT_SAMPLE_BITS,
        },
};


static inline void pcm_close_device_handler() {
    if (PcmPlayer.device_info.device_handler != NULL) {
        rt_device_close(PcmPlayer.device_info.device_handler);
        PcmPlayer.device_info.device_handler = NULL;
    }
}


static inline void pcm_delete_mem_pool() {
    if (PcmPlayer.buffer_pool != NULL) {
        rt_mp_delete(PcmPlayer.buffer_pool);
        PcmPlayer.buffer_pool = NULL;
    }
}

static inline void pcm_delete_mq() {
    if (PcmPlayer.buffer_queue != NULL) {
        rt_mq_delete(PcmPlayer.buffer_queue);
        PcmPlayer.buffer_queue = NULL;
    }
}

static inline void pcm_delete_event() {
    if (PcmPlayer.play_event != NULL) {
        rt_event_delete(PcmPlayer.play_event);
        PcmPlayer.play_event = NULL;
    }
}

static inline void pcm_delete_mutex() {
    if (PcmPlayer.play_event_callback_lock != NULL) {
        rt_mutex_delete(PcmPlayer.play_event_callback_lock);
        PcmPlayer.play_event_callback_lock = NULL;
    }
}


static inline bool take_event(rt_event_t ev, const uint32_t set, const int32_t timeout) {
    return rt_event_recv(ev, set, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, timeout, NULL) == RT_EOK;
}

static inline size_t write_bytes_to_device(const char *bytes, const size_t size) {
    return rt_device_write(PcmPlayer.device_info.device_handler, 0, bytes, size);
}

static inline void on_play_event(const PcmPlayEvent event) {
    rt_kprintf("pcm play event is : %d!!!!!!\n  ", event);
    if (rt_mutex_take(PcmPlayer.play_event_callback_lock, 50 * 10) == RT_EOK) {
        if (PcmPlayer.play_event_callback != NULL) {
            PcmPlayer.play_event_callback(event);
        }
        rt_mutex_release(PcmPlayer.play_event_callback_lock);
    }
}

void pcm_player_play() {
    rt_event_send(PcmPlayer.play_event, PCM_PLAY_START);
}

void pcm_player_pause() {
    rt_event_send(PcmPlayer.play_event, PCM_PLAY_PAUSE);
}

void pcm_player_stop() {
    rt_event_send(PcmPlayer.play_event, PCM_PLAY_STOP);
}

void pcm_player_set_volume(int volume) {
    if (PcmPlayer.device_info.device_handler != NULL) {
        if (volume < PCM_PLAYER_MIN_VOLUME) {
            volume = PCM_PLAYER_MIN_VOLUME;
        } else if (volume > PCM_PLAYER_MAX_VOLUME) {
            volume = PCM_PLAYER_MAX_VOLUME;
        }
        PcmPlayer.device_info.volume = volume;
        struct rt_audio_caps caps = {
                .main_type = AUDIO_TYPE_MIXER,
                .sub_type = AUDIO_MIXER_VOLUME,
                .udata.value = volume,
        };
        rt_device_control(PcmPlayer.device_info.device_handler, AUDIO_CTL_CONFIGURE, &caps);
    }
}

int pcm_player_volume() {
    return PcmPlayer.device_info.volume;
}

static bool pcm_open_device() {
    //打开设备
    rt_err_t ret = rt_device_open(PcmPlayer.device_info.device_handler, RT_DEVICE_OFLAG_WRONLY);
    if (ret != RT_EOK) {
        return false;
    }

    //配置音频参数
    struct rt_audio_caps caps = {
            .udata.config.samplerate = PcmPlayer.audio_info.sample_rate,
            .udata.config.channels = PcmPlayer.audio_info.channel,
            .udata.config.samplebits = PcmPlayer.audio_info.sample_bits,
    };

    caps.main_type = AUDIO_TYPE_OUTPUT;
    caps.sub_type = AUDIO_DSP_PARAM;
    rt_device_control(PcmPlayer.device_info.device_handler, AUDIO_CTL_CONFIGURE, &caps);

    caps.main_type = AUDIO_TYPE_MIXER;
    caps.sub_type = AUDIO_MIXER_VOLUME;
    caps.udata.value = PcmPlayer.device_info.volume;
    rt_device_control(PcmPlayer.device_info.device_handler, AUDIO_CTL_CONFIGURE, &caps);

    return true;
}

static void pcm_close_device() {
    pcm_close_device_handler();
}


void pcm_player_clear() {
    pcm_close_device_handler();
    pcm_delete_mem_pool();
    pcm_delete_mq();
    pcm_delete_event();
    pcm_delete_mutex();
}


void pcm_player_config(int sample_rate, int channel, int sample_bits) {
    rt_kprintf("sample_rate:%d,channels:%d,sample_bits:%d!!!!!!!!\n", sample_rate, channel, sample_bits);
    PcmPlayer.audio_info.sample_rate = sample_rate;
    PcmPlayer.audio_info.channel = channel;
    PcmPlayer.audio_info.sample_bits = sample_rate;
}

void pcm_player_register_on_play_event(OnPcmPlayerEventCallback callback) {
    if (rt_mutex_take(PcmPlayer.play_event_callback_lock, RT_WAITING_FOREVER) == RT_EOK) {
        PcmPlayer.play_event_callback = callback;
        rt_mutex_release(PcmPlayer.play_event_callback_lock);
    }
}

AllocBytes pcm_player_alloc_bytes() {
    AllocBytes allocated = {
            .flag = ALLOC_BYTES_FLAG_ERROR,
            .size = 0,
            .bytes = NULL,
    };

    if (PcmPlayer.buffer_pool == NULL) {
        return allocated;
    }

    char *bytes = rt_mp_alloc(PcmPlayer.buffer_pool, RT_WAITING_FOREVER);
    if (bytes == NULL) {
        return allocated;
    }

    allocated.flag = ALLOC_BYTES_FLAG_OK;
    allocated.size = PCM_AUDIO_MEM_POOL_BLOCK_SIZE;
    allocated.bytes = bytes;
    return allocated;
}

rt_err_t pcm_player_write_bytes(AllocBytes allocated) {
    if (PcmPlayer.buffer_queue == NULL) {
        rt_mp_free(allocated.bytes);
        return RT_ERROR;
    }
    rt_kprintf("pcm_player_write_bytes!!!!!!!!!\n");
    return rt_mq_send(PcmPlayer.buffer_queue, (void *) &allocated, sizeof(AllocBytes));
}


static void pcm_player_entry(void *param) {
    while (1) {
        if (take_event(PcmPlayer.play_event, PCM_PLAY_START, 50)) {
            if (pcm_open_device()) {
                on_play_event(PCM_PLAY_START);
                while (1) {
                    if (take_event(PcmPlayer.play_event, PCM_PLAY_PAUSE, 0)) {
                        on_play_event(PCM_PLAY_PAUSE);
                        break;
                    }
                    if (take_event(PcmPlayer.play_event, PCM_PLAY_STOP, 0)) {
                        on_play_event(PCM_PLAY_STOP);
                        pcm_close_device();
                        break;
                    }
                    AllocBytes allocated;
                    rt_err_t e = rt_mq_recv(PcmPlayer.buffer_queue, &allocated, sizeof(AllocBytes), 0);
                    if (e == RT_EOK) {
                        write_bytes_to_device(allocated.bytes, allocated.size);
                        rt_kprintf("write play bytes!!!!!!!!\n");
                        if (allocated.flag == ALLOC_BYTES_LAST_LAST) {
                            on_play_event(PCM_PLAY_FINISH);
                            pcm_close_device();
                        }
                        rt_mp_free(allocated.bytes);
                    } else {
                        LOG_D("take bytes from PcmPlayer's buffer queue error");
                    }
                }
            } else {
                on_play_event(PCM_PLAY_ERROR);
            }
        } else {
            rt_thread_sleep(50);
        }
    }
}

bool pcm_player_init() {
    //寻找设备
    PcmPlayer.device_info.device_handler = rt_device_find(PcmPlayer.device_info.device_name);
    if (PcmPlayer.device_info.device_handler == NULL) {
        goto __exit;
    }
    rt_kprintf("find device successfully!!!!!!!!!\n");

    //内存池
    PcmPlayer.buffer_pool = rt_mp_create(PCM_AUDIO_MEM_POOL_NAME, PCM_AUDIO_MEM_POOL_BLOCK_COUNT,
                                         PCM_AUDIO_MEM_POOL_BLOCK_SIZE);
    if (PcmPlayer.buffer_pool == NULL) {
        goto __exit;
    }
    rt_kprintf("mem pool init successfully!!!!!!!!!\n");

    //消息队列
    PcmPlayer.buffer_queue = rt_mq_create(PCM_AUDIO_MQ_NAME, sizeof(AllocBytes), PCM_AUDIO_MQ_MAX_COUNT,
                                          RT_IPC_FLAG_FIFO);
    if (PcmPlayer.buffer_queue == NULL) {
        goto __exit;
    }
    rt_kprintf("mq init successfully!!!!!!!!!\n");

    PcmPlayer.play_event = rt_event_create(PCM_AUDIO_PLAY_EVENT_NAME, RT_IPC_FLAG_FIFO);

    if (PcmPlayer.play_event == NULL) {
        goto __exit;
    }
    rt_kprintf("event init successfully!!!!!!!!!\n");

    PcmPlayer.play_event_callback_lock = rt_mutex_create(PCM_PLAYER_ON_PLAYE_EVENT_MUTEX_NAME, RT_IPC_FLAG_FIFO);
    if (PcmPlayer.play_event_callback_lock == NULL) {
        goto __exit;
    }
    rt_kprintf("mutex init successfully!!!!!!!!!\n");

    rt_thread_t tid = rt_thread_create(PCM_PLAYER_LOOP_THREAD_NAME, pcm_player_entry, NULL,
                                       PCM_PLAYER_THREAD_STACK_SIZE,
                                       PCM_PLAYER_THREAD_PRIORITY, PCM_PLAYER_THREAD_TICK);

    if (tid == NULL) {
        rt_kprintf("tid is null!!!!!!!!!\n");
        goto __exit;
    }

    rt_kprintf("create thread successfully!!!!!!!!!\n");

    rt_thread_startup(tid);
    rt_kprintf("start pcm thread successfully!!!!!!!!!\n");

    return true;

    __exit:
    pcm_player_clear();
    return false;
}



