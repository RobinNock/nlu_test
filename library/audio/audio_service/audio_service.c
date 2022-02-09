//
// Created by 202106 on 2021/11/18.
//

#include "audio_service.h"
#include <sys/cdefs.h>

static struct rt_event sServiceEvent;
static struct rt_messagequeue sServiceMq;
static struct rt_mailbox sServiceMailbox;
static char sServiceMqPool[AUDIO_SERVICE_MQ_POOL_SIZE];
static char sServiceMailboxPool[AUDIO_SERVICE_MQ_POOL_SIZE];

static Channel sRawChannel = {
        .id = 0,
        .source_ptr = NULL,
};

static Channel sMediaChannel = {
        .id = 0,
        .source_ptr = NULL,
};

inline void close_audio_source(ChannelPtr channel_ptr) {
    if (channel_ptr->source_ptr != NULL) {
        if (channel_ptr->source_ptr->close != NULL) {
            channel_ptr->source_ptr->close(channel_ptr->source_ptr, channel_ptr->id);
            channel_ptr->source_ptr = NULL;
        }
    }
}

static void audio_service_loop_entry(void *parm) {
    while (1) {
        AudioSourcePtr ptr;
        if (rt_mb_recv(&sServiceMailbox, &ptr, RT_WAITING_FOREVER) == RT_EOK) {
            if (ptr->priority == AUDIO_SOURCE_PRIORITY_RAW) {
                MediaAudioPlayer.pause();
                RawAudioPlayer.stop();
                rt_mutex_take(&sRawChannel.lock, RT_WAITING_FOREVER);
                close_audio_source(&sRawChannel);
                sRawChannel.source_ptr = ptr;
                if (++sRawChannel.id <= 0) {
                    ++sRawChannel.id;
                }
                const AudioSourceFrom from = sRawChannel.source_ptr->from;
                if (from == AUDIO_SOURCE_FROM_ONLINE) {
                    RawAudioPlayer.set_url(sRawChannel.source_ptr->provider.url);
                    RawAudioPlayer.play();
                } else if (from == AUDIO_SOURCE_FROM_LOCAL) {
                }
                rt_mutex_release(&sRawChannel.lock);
            } else if (ptr->priority == AUDIO_SOURCE_PRIORITY_MEDIA) {
                MediaAudioPlayer.stop();
                rt_mutex_take(&sMediaChannel.lock, RT_WAITING_FOREVER);
                close_audio_source(&sMediaChannel);
                sMediaChannel.source_ptr = ptr;
                if (++sMediaChannel.id <= 0) {
                    ++sMediaChannel.id;
                }
                const AudioSourceFrom from = sMediaChannel.source_ptr->from;
                if (from == AUDIO_SOURCE_FROM_ONLINE) {
                    MediaAudioPlayer.set_url(sMediaChannel.source_ptr->provider.url);
                    MediaAudioPlayer.play();
                } else if (from == AUDIO_SOURCE_FROM_LOCAL) {
                }

                rt_mutex_release(&sMediaChannel.lock);
            }
        }

        rt_thread_sleep(50);
    }
}

static void on_raw_play_event(const PlayStateEvent event) {
    rt_mutex_take(&sRawChannel.lock, RT_WAITING_FOREVER);
    if (sRawChannel.source_ptr != NULL) {
        if (sRawChannel.source_ptr->on_play_event != NULL) {
            sRawChannel.source_ptr->on_play_event(event);
        }
    }
    if (event == PLAY_EVENT_FINISH) {
        close_audio_source(&sRawChannel);
    }
    rt_mutex_release(&sRawChannel.lock);
}

static void on_media_play_event(const PlayStateEvent event) {
    rt_mutex_take(&sMediaChannel.lock, RT_WAITING_FOREVER);
    if (sMediaChannel.source_ptr != NULL) {
        if (sMediaChannel.source_ptr->on_play_event != NULL) {
            sMediaChannel.source_ptr->on_play_event(event);
        }
    }
    if (event == PLAY_EVENT_FINISH) {
        close_audio_source(&sMediaChannel);
    } else if (event == PLAY_EVENT_FAIL) {
        close_audio_source(&sMediaChannel);
    }
    rt_mutex_release(&sMediaChannel.lock);
}

static void play_media() {
    MediaAudioPlayer.play();
}

static void play_raw() {
    RawAudioPlayer.play();
}

static void pause_media() {
    MediaAudioPlayer.pause();
}

static void pause_raw() {
    RawAudioPlayer.pause();
}

static void stop_media() {
    MediaAudioPlayer.stop();
}

static void stop_raw() {
    RawAudioPlayer.stop();
}

static void set_media_volume(const int volume) {
    MediaAudioPlayer.set_volume(volume);
}

static void set_raw_volume(const int volume) {
    RawAudioPlayer.set_volume(volume);
}

static int media_volume() {
    return MediaAudioPlayer.volume();
}

static int raw_volume() {
    return RawAudioPlayer.volume();
}

static bool init() {
    if (rt_event_init(&sServiceEvent, AUDIO_SERVICE_EVENT_NAME, RT_IPC_FLAG_FIFO) != RT_EOK) {
        return false;
    }

    if (rt_mutex_init(&sRawChannel.lock, AUDIO_TTS_MUTEX_NAME, RT_IPC_FLAG_FIFO) != RT_EOK) {
        return false;
    }

    if (rt_mutex_init(&sMediaChannel.lock, AUDIO_MEDIA_MUTEX_NAME, RT_IPC_FLAG_FIFO) != RT_EOK) {
        return false;
    }

    if (rt_mq_init(&sServiceMq, AUDIO_SERVICE_MQ_NAME, sServiceMqPool, sizeof(Channel), AUDIO_SERVICE_MQ_POOL_SIZE,
                   RT_IPC_FLAG_FIFO) != RT_EOK) {
        return false;
    }

    if (rt_mb_init(&sServiceMailbox, AUDIO_SERVICE_MAILBOX_NAME, sServiceMailboxPool, AUDIO_SERVICE_MQ_POOL_SIZE,
                   RT_IPC_FLAG_FIFO) != RT_EOK) {
        return false;
    }

    MediaAudioPlayer.init();
    MediaAudioPlayer.register_on_play_event(on_media_play_event);

    RawAudioPlayer.init();
    RawAudioPlayer.register_on_play_event(on_raw_play_event);

    rt_thread_t tid = rt_thread_create(AUDIO_SERVICE_LOOP_NAME, audio_service_loop_entry, NULL,
                                       AUDIO_SERVICE_LOOP_STACK_SIZE,
                                       AUDIO_SERVICE_LOOP_PRIORITY, AUDIO_SERVICE_LOOP_TICK);
    if (tid == NULL) {
        return false;
    }

    rt_thread_startup(tid);
}

static size_t post(AudioSourcePtr ptr) {
    rt_mb_send(&sServiceMailbox, ptr);
    return 0;
}

const _AudioService AudioService = {
        .init = init,
        .post = post,
        .play_media = play_media,
        .play_raw = play_raw,
        .pause_media = pause_media,
        .pause_raw = pause_raw,
        .stop_media = stop_media,
        .stop_raw = stop_raw,
        .set_media_volume = set_media_volume,
        .set_raw_volume = set_raw_volume,
        .media_volume = media_volume,
        .raw_volume = raw_volume,
};
