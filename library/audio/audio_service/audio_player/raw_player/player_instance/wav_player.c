/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Date           Author       Notes
 * 2019-07-15     Zero-Free    first implementation
 */


#include "wav_hdr.h"
#include "wav_player.h"

typedef struct {
    int type;
    void *data;
} PlayMsg;

typedef struct {
    int state;
    char *url;
    char *buffer;
    rt_device_t device;
    rt_mq_t mq;
    rt_mutex_t lock;
    struct rt_completion ack;
    FILE *fp;
    int volume;

    void (*callback)(enum PLAYER_STATE status);
} WavPlayer, *WavPlayerPtr;

static WavPlayer sWavPlayer;

enum MSG_TYPE {
    MSG_NONE = 0,
    MSG_START = 1,
    MSG_STOP = 2,
    MSG_PAUSE = 3,
    MSG_RESUME = 4,
    MSG_FINISH = 5,
};

enum PLAYER_EVENT {
    PLAYER_EVENT_NONE = 0,
    PLAYER_EVENT_PLAY = 1,
    PLAYER_EVENT_STOP = 2,
    PLAYER_EVENT_PAUSE = 3,
    PLAYER_EVENT_RESUME = 4,
    PLAYER_EVENT_FINISH = 5,
};

static void play_lock(void) {
    rt_mutex_take(sWavPlayer.lock, RT_WAITING_FOREVER);
}

static void play_unlock(void) {
    rt_mutex_release(sWavPlayer.lock);
}

static rt_err_t play_msg_send(WavPlayerPtr player, int type, void *data) {
    PlayMsg msg = {
            .type = type,
            .data = data,
    };

    return rt_mq_send(player->mq, &msg, sizeof(PlayMsg));
}

int wav_player_play() {
    rt_err_t result = RT_EOK;

    rt_completion_init(&sWavPlayer.ack);

    play_lock();
    if (sWavPlayer.state != PLAYER_STATE_STOPPED) {
        wav_player_stop();
    }
    result = play_msg_send(&sWavPlayer, MSG_START, RT_NULL);
    rt_completion_wait(&sWavPlayer.ack, RT_WAITING_FOREVER);
    play_unlock();

    return result;
}

int wav_player_stop(void) {
    rt_err_t result = RT_EOK;

    rt_completion_init(&sWavPlayer.ack);

    play_lock();
    if (sWavPlayer.state != PLAYER_STATE_STOPPED) {
        result = play_msg_send(&sWavPlayer, MSG_STOP, RT_NULL);
        rt_completion_wait(&sWavPlayer.ack, RT_WAITING_FOREVER);
    }
    play_unlock();

    return result;
}

int wav_player_pause(void) {
    rt_err_t result = RT_EOK;

    rt_completion_init(&sWavPlayer.ack);
    play_lock();
    if (sWavPlayer.state == PLAYER_STATE_STARTED) {
        result = play_msg_send(&sWavPlayer, MSG_PAUSE, RT_NULL);
        rt_completion_wait(&sWavPlayer.ack, RT_WAITING_FOREVER);
    }
    play_unlock();

    return result;
}

int wav_player_resume(void) {
    rt_err_t result = RT_EOK;

    rt_completion_init(&sWavPlayer.ack);

    play_lock();
    if (sWavPlayer.state == PLAYER_STATE_PAUSED) {
        result = play_msg_send(&sWavPlayer, MSG_RESUME, RT_NULL);
        rt_completion_wait(&sWavPlayer.ack, RT_WAITING_FOREVER);
    }
    play_unlock();

    return result;
}

int wav_player_set_volume(int volume) {
    pcm_player_set_volume(volume);
}

int wav_player_volume(void) {
    return pcm_player_volume();
}

int wav_player_state(void) {
    return sWavPlayer.state;
}

char *wav_player_url(void) {
    return sWavPlayer.url;
}

static rt_err_t wavplayer_open(WavPlayerPtr player) {
    rt_err_t result = RT_EOK;

    /* find device */
    player->device = rt_device_find(PCM_PLAYER_DEVICE_HARDWARE_NAME);
    if (player->device == RT_NULL) {
        return -RT_ERROR;
    }

    /* open file */
    player->fp = fopen(player->url, "rb");
    if (player->fp == RT_NULL) {
        result = -RT_ERROR;
        goto __exit;
    }

    struct wav_header wav;
    wavheader_read(&wav, player->fp);

    result = rt_device_open(player->device, RT_DEVICE_OFLAG_WRONLY);
    if (result != RT_EOK) {
        goto __exit;
    }

    struct rt_audio_caps caps = {
            .udata.config.samplerate = wav.fmt_sample_rate,
            .udata.config.channels = wav.fmt_channels,
            .udata.config.samplebits = wav.fmt_bit_per_sample,
    };

    caps.main_type = AUDIO_TYPE_OUTPUT;
    caps.sub_type = AUDIO_DSP_PARAM;
    rt_device_control(player->device, AUDIO_CTL_CONFIGURE, &caps);
    /* set volume according to configuration */
    caps.main_type = AUDIO_TYPE_MIXER;
    caps.sub_type = AUDIO_MIXER_VOLUME;
    caps.udata.value = player->volume;
    rt_device_control(player->device, AUDIO_CTL_CONFIGURE, &caps);

    return RT_EOK;

    __exit:
    if (player->fp) {
        fclose(player->fp);
        player->fp = RT_NULL;
    }

    if (player->device) {
        rt_device_close(player->device);
        player->device = RT_NULL;
    }

    return result;
}


static void wavplayer_close(WavPlayerPtr player) {
    if (player->fp) {
        fclose(player->fp);
        player->fp = RT_NULL;
    }

    if (player->device) {
        rt_device_close(player->device);
        player->device = RT_NULL;
    }
}

static int wavplayer_event_handler(WavPlayerPtr player, int timeout) {
    PlayMsg msg;
    rt_err_t result = rt_mq_recv(player->mq, &msg, sizeof(PlayMsg), timeout);

    int event = PLAYER_EVENT_NONE;
    if (result != RT_EOK) {
        return event;
    }

    switch (msg.type) {
        case MSG_START:
            event = PLAYER_EVENT_PLAY;
            player->state = PLAYER_STATE_STARTED;
            if (sWavPlayer.callback != NULL) {
                sWavPlayer.callback(PLAYER_STATE_STARTED);
            }
            break;

        case MSG_STOP:
            event = PLAYER_EVENT_STOP;
            player->state = PLAYER_STATE_STOPPED;
            if (sWavPlayer.callback != NULL) {
                sWavPlayer.callback(PLAYER_STATE_STOPPED);
            }
            break;

        case MSG_PAUSE:
            event = PLAYER_EVENT_PAUSE;
            player->state = PLAYER_STATE_PAUSED;
            if (sWavPlayer.callback != NULL) {
                sWavPlayer.callback(PLAYER_STATE_PAUSED);
            }
            break;

        case MSG_RESUME:
            event = PLAYER_EVENT_RESUME;
            player->state = PLAYER_STATE_STARTED;
            if (sWavPlayer.callback != NULL) {
                sWavPlayer.callback(PLAYER_STATE_STARTED);
            }
            break;

        default:
            event = PLAYER_EVENT_NONE;
            break;
    }

    rt_completion_done(&player->ack);

    return event;
}

static void wavplayer_entry(void *parameter) {
    rt_err_t result = RT_EOK;
    while (1) {
        /* wait play event forever */
        int event = wavplayer_event_handler(&sWavPlayer, RT_WAITING_FOREVER);
        if (event != PLAYER_EVENT_PLAY) {
            continue;
        }

        /* open wavplayer */
        result = wavplayer_open(&sWavPlayer);
        if (result != RT_EOK) {
            sWavPlayer.state = PLAYER_STATE_FAILED;
            if (sWavPlayer.callback != NULL) {
                sWavPlayer.callback(PLAYER_STATE_FAILED);
            }
            continue;
        }

        while (1) {
            event = wavplayer_event_handler(&sWavPlayer, RT_WAITING_NO);

            switch (event) {
                case PLAYER_EVENT_NONE: {
                    /* read raw data from file stream */
                    rt_int32_t size = fread(sWavPlayer.buffer, WAV_PLAYER_BUFFER_SIZE, 1, sWavPlayer.fp);
                    if (size != 1) {
                        /* FILE END*/
                        sWavPlayer.state = PLAYER_STATE_FINISHED;
                        if (sWavPlayer.callback != NULL) {
                            sWavPlayer.callback(PLAYER_STATE_FINISHED);
                        }
                    } else {
                        /*witte data to sound device*/
                        rt_device_write(sWavPlayer.device, 0, sWavPlayer.buffer, WAV_PLAYER_BUFFER_SIZE);
                    }
                    break;
                }

                case PLAYER_EVENT_PAUSE: {
                    /* wait resume or stop event forever */
                    event = wavplayer_event_handler(&sWavPlayer, RT_WAITING_FOREVER);
                    break;
                }
                default:
                    break;
            }

            if (sWavPlayer.state == PLAYER_STATE_STOPPED || sWavPlayer.state == PLAYER_STATE_FINISHED) {
                break;
            }
        }

        /* close wavplayer */
        wavplayer_close(&sWavPlayer);
    }
}


void wav_player_set_event_callback(void (*callback)(enum PLAYER_STATE status)) {
    play_lock();
    sWavPlayer.callback = callback;
    play_unlock();
}

void wav_player_set_url(const char *url) {
    play_lock();
    wav_player_stop();
    if (sWavPlayer.url != NULL) {
        rt_free(sWavPlayer.url);
    }
    sWavPlayer.url = rt_strdup(url);
    play_unlock();
}

int wav_player_init(void) {

    sWavPlayer.callback = NULL;
    sWavPlayer.buffer = rt_malloc(WAV_PLAYER_BUFFER_SIZE);
    if (sWavPlayer.buffer == RT_NULL) {
        goto __exit;
    }
    rt_memset(sWavPlayer.buffer, 0, WAV_PLAYER_BUFFER_SIZE);

    sWavPlayer.mq = rt_mq_create("wav_p", 10, sizeof(PlayMsg), RT_IPC_FLAG_FIFO);
    if (sWavPlayer.mq == RT_NULL) {
        goto __exit;
    }

    sWavPlayer.lock = rt_mutex_create("wav_p", RT_IPC_FLAG_FIFO);
    if (sWavPlayer.lock == RT_NULL) {
        goto __exit;
    }

    sWavPlayer.volume = WAV_PLAYER_VOLUME_DEFAULT;

    rt_thread_t tid = rt_thread_create("wav_p",
                                       wavplayer_entry,
                                       RT_NULL,
                                       WAV_PLAYER_THREAD_STACK_SIZE,
                                       WAV_PLAYER_THREAD_PRIORITY, 10);
    if (tid != NULL) {
        rt_thread_startup(tid);
    }

    return RT_EOK;

    __exit:
    if (sWavPlayer.buffer) {
        rt_free(sWavPlayer.buffer);
        sWavPlayer.buffer = RT_NULL;
    }

    if (sWavPlayer.mq) {
        rt_mq_delete(sWavPlayer.mq);
        sWavPlayer.mq = RT_NULL;
    }

    if (sWavPlayer.lock) {
        rt_mutex_delete(sWavPlayer.lock);
        sWavPlayer.lock = RT_NULL;
    }

    return RT_ERROR;
}

//INIT_APP_EXPORT(wav_player_init);
