#include <sys/cdefs.h>
//
// Created by 202106 on 2021/11/18.
//

#include "raw_player.h"

static PlayEventCallback sPlayEventCallback = NULL;
static struct rt_mutex sPlayEventCallbackMutex;
static struct rt_mailbox sPlayEventMailbox;
static char sPlayEventMailboxPool[AUDIO_WAV_PLAYER_ON_PLAY_EVENT_MAILBOX_SIZE];

static void dispatch_play_event(enum PLAYER_STATE state) {
    PlayStateEvent pe = PLAY_EVENT_UNKNOWN;
    if (state == PLAYER_STATE_STARTED) {
        pe = PLAY_EVENT_START;
    } else if (state == PLAYER_STATE_PAUSED) {
        pe = PLAY_EVENT_PAUSE;
    } else if (state == PLAYER_STATE_STOPPED) {
        pe = PLAY_EVENT_FINISH;
    } else if (state == PLAYER_STATE_FAILED) {
        pe = PLAY_EVENT_FAIL;
    }
    rt_mb_send(&sPlayEventMailbox, pe);
}

static void play_event_loop_entry(void *param) {
    while (1) {
        PlayStateEvent pe = PLAY_EVENT_UNKNOWN;
        rt_mb_recv(&sPlayEventMailbox, &pe, RT_WAITING_FOREVER);
        rt_mutex_take(&sPlayEventCallbackMutex, RT_WAITING_FOREVER);
        if (sPlayEventCallback != NULL) {
            sPlayEventCallback(pe);
        }
        rt_mutex_release(&sPlayEventCallbackMutex);
    }
}

static bool init() {

    pcm_player_init();

    if (wav_player_init() != RT_EOK) {
        return false;
    }

    wav_player_set_volume(AUDIO_RAW_VOLUME);
    wav_player_set_event_callback(dispatch_play_event);

    if (rt_mutex_init(&sPlayEventCallbackMutex, AUDIO_WAV_PLAYER_ON_PLAY_EVENT_CALLBACK_MUTEX_NAME, RT_IPC_FLAG_FIFO) !=
        RT_EOK) {
        return false;
    }

    if (rt_mb_init(&sPlayEventMailbox, AUDIO_WAV_PLAYER_ON_PLAY_EVENT_MQ, sPlayEventMailboxPool,
                   AUDIO_WAV_PLAYER_ON_PLAY_EVENT_MAILBOX_SIZE, RT_IPC_FLAG_FIFO) != RT_EOK) {
        return false;
    }

    rt_thread_t tid = rt_thread_create(AUDIO_RAW_PLAYER_ON_PLAY_LOOP_NAME, play_event_loop_entry, NULL,
                                       AUDIO_RAW_PLAYER_ON_PLAY_LOOP_STACK_SIZE, AUDIO_RAW_PLAYER_ON_PLAY_LOOP_PRIORITY,
                                       AUDIO_RAW_PLAYER_ON_PLAY_LOOP_TICK);
    if (tid == NULL) {
        return false;
    }

    rt_thread_startup(tid);

    return true;
}

static void register_on_play_event(const PlayEventCallback callback) {
    rt_mutex_take(&sPlayEventCallbackMutex, RT_WAITING_FOREVER);
    sPlayEventCallback = callback;
    rt_mutex_release(&sPlayEventCallbackMutex);
}

static void set_volume(const int volume) {
    wav_player_set_volume(volume);
}

static void set_url(const char *url) {
    wav_player_set_url(url);
}

static void play() {
    wav_player_play();
}

static void pause() {
    wav_player_pause();
}

static void stop() {
    wav_player_stop();
}

static PlayStateEvent status() {
    switch (wav_player_state()) {
        case PLAYER_STATE_STARTED:
            return PLAY_EVENT_START;
        case PLAYER_STATE_PAUSED:
            return PLAY_EVENT_PAUSE;
        case PLAYER_STATE_STOPPED:
        case PLAYER_STATE_FAILED:
        case PLAYER_STATE_FINISHED:
            return PLAY_EVENT_STOP;
        default:
            return PLAY_EVENT_UNKNOWN;
    }
}

static int volume() {
    wav_player_volume();
}

const AudioPlayer RawAudioPlayer = {
        .init = init,
        .set_url = set_url,
        .set_volume  = set_volume,
        .register_on_play_event = register_on_play_event,
        .play = play,
        .pause = pause,
        .stop = stop,
        .status = status,
        .volume = volume,
};


