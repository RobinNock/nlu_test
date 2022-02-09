//
// Created by 202106 on 2021/11/18.
//

#include "media_player.h"


static PlayEventCallback sPlayEventCallback = NULL;
static struct rt_mutex sPlayEventCallbackMutex;
static struct rt_mailbox sPlayEventMailBox;
static char sPlayEventMailBoxPool[AUDIO_LIB_PLAYER_ON_PLAY_EVENT_MAILBOX_SIZE];

/**
 * 分发libplayer的播放事件
 * @param event
 * @param user_data
 */
static void dispatch_play_event(int event, void *_) {
    PlayStateEvent pe = PLAY_EVENT_UNKNOWN;
    if (event == PLAYER_AUDIO_PLAYBACK) {//start
        pe = PLAY_EVENT_START;
    } else if (event == PLAYER_APP_RESUMED) {//start
        pe = PLAY_EVENT_START;
    } else if (event == PLAYER_AUDIO_CLOSED) {//finish
        pe = PLAY_EVENT_FINISH;
    } else if (event == PLAYER_PLAYBACK_FAILED) {//fail
        pe = PLAY_EVENT_FAIL;
    } else if (event == PLAYER_PLAYBACK_BREAK) {//fail
        pe = PLAY_EVENT_FAIL;
    } else if (event == PLAYER_PLAYBACK_STOP) {//stop
        pe = PLAY_EVENT_STOP;
    } else if (event == PLAYER_APP_SUSPENDED) {//pause
        pe = PLAY_EVENT_PAUSE;
    }

    rt_mb_send(&sPlayEventMailBox, pe);
}

/**
 * 播放事件分发处理线程
 * @param param
 */
static void play_event_loop_entry(void *param) {
    while (1) {
        PlayStateEvent pe = PLAY_EVENT_UNKNOWN;
        rt_mb_recv(&sPlayEventMailBox, &pe, RT_WAITING_FOREVER);
        rt_mutex_take(&sPlayEventCallbackMutex, RT_WAITING_FOREVER);
        if (sPlayEventCallback != NULL) {
            sPlayEventCallback(pe);
        }
        rt_mutex_release(&sPlayEventCallbackMutex);
        rt_thread_sleep(50);
    }
}

/**
 * 初始化libplayer,事件线程,消息队列,互斥量
 * @return
 */
static bool init() {
    player_set_device_name(AUDIO_LIB_PLAYER_HARDWARE_NAME);

    player_codec_helixmp3_register();
    player_codec_helixaac_register();
    player_format_mp4ff_register();

    if (player_system_init() != PLAYER_OK) {
        return false;
    }

    if (rt_mutex_init(&sPlayEventCallbackMutex, AUDIO_LIB_PLAYER_ON_PLAY_EVENT_CALLBACK_MUTEX_NAME, RT_IPC_FLAG_FIFO) !=
        RT_EOK) {
        return false;
    }

    if (rt_mb_init(&sPlayEventMailBox, AUDIO_LIB_PLAYER_ON_PLAY_EVENT_MQ_NAME, sPlayEventMailBoxPool,
                   AUDIO_LIB_PLAYER_ON_PLAY_EVENT_MAILBOX_SIZE, RT_IPC_FLAG_FIFO) != RT_EOK) {
        return false;
    }

    player_set_volume(AUDIO_MEDIA_VOLUME);
    player_set_event_callback(dispatch_play_event, NULL);


    rt_thread_t tid = rt_thread_create(AUDIO_LIB_PLAYER_ON_PLAY_LOOP_NAME, play_event_loop_entry, NULL,
                                       AUDIO_LIB_PLAYER_ON_PLAY_LOOP_STACK_SIZE, AUDIO_LIB_PLAYER_ON_PLAY_LOOP_PRIORITY,
                                       AUDIO_LIB_PLAYER_ON_PLAY_LOOP_TICK);
    if (tid == NULL) {
        return false;
    }

    rt_thread_startup(tid);

    return true;
}

/**
 * 注册播放事件回调函数
 * @param callback
 */
static void register_on_play_event(const PlayEventCallback callback) {
    rt_mutex_take(&sPlayEventCallbackMutex, RT_WAITING_FOREVER);
    sPlayEventCallback = callback;
    rt_mutex_release(&sPlayEventCallbackMutex);
}

/**
 * 设置音量
 * ps:设置的是虚拟声卡pcm0的增益,非实际音量
 * @param volume
 */
static void set_volume(const int volume) {
    player_set_volume(volume);
}

static void set_url(const char *url) {
    player_set_uri(url);
}

static void play() {
    player_play();
}

static void pause() {
    player_pause();
}

static void stop() {
    player_stop();
}

static PlayStateEvent status() {
    switch (player_get_state()) {
        case PLAYER_STAT_PLAYING:
            return PLAY_EVENT_START;
        case PLAYER_STAT_PAUSED:
            return PLAY_EVENT_PAUSE;
        case PLAYER_STAT_STOPPED:
            return PLAY_EVENT_STOP;
        default:
            return PLAY_EVENT_UNKNOWN;
    }
}

static int volume() {
    return player_get_volume();
}

const AudioPlayer MediaAudioPlayer = {
        .init = init,
        .set_url = set_url,
        .set_volume = set_volume,
        .play = play,
        .pause = pause,
        .stop = stop,
        .status = status,
        .register_on_play_event = register_on_play_event,
        .volume = volume,
};