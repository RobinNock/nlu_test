//
// Created by 202106 on 2021/11/18.
//

#include "audio_service.h"
#include <finsh.h>

void audio_service_init() {
    AudioService.init();
}

MSH_CMD_EXPORT(audio_service_init, test media init)

void media_play_event(PlayStateEvent event) {
    rt_kprintf("=======================!!!!! >> Event is %d << !!!!=======================\n", event);
}

void close_audio_source(AudioSourcePtr ptr, const size_t id) {
    if (ptr->from == AUDIO_SOURCE_FROM_ONLINE) {
        rt_free(ptr->provider.url);
    }
    rt_free(ptr);
}

void test_media() {
    AudioSourcePtr ptr = rt_malloc(sizeof(AudioSource));
    ptr->priority = AUDIO_SOURCE_PRIORITY_MEDIA;
    ptr->from = AUDIO_SOURCE_FROM_ONLINE;
    ptr->on_play_event = media_play_event;
    ptr->provider.url = "http://192.168.0.102:8080/files/hong.mp3";
    size_t id = AudioService.post(ptr);
    rt_kprintf("!!!!!!!!!post media id is:%ld\n", id);
}

MSH_CMD_EXPORT(test_media, test media)

void test_media_url(int argc, char *argv[]) {
    if (argc < 2) {
        rt_kprintf("please input url!!!\n");
        return;
    }
    char *url = strdup(argv[1]);
    rt_kprintf("url is %s!!!\n", argv[1]);
    AudioSourcePtr ptr = rt_malloc(sizeof(AudioSource));
    ptr->priority = AUDIO_SOURCE_PRIORITY_MEDIA;
    ptr->from = AUDIO_SOURCE_FROM_ONLINE;
    ptr->on_play_event = media_play_event;
    ptr->provider.url = url;
    ptr->close = NULL;
    AudioService.post(ptr);
}

MSH_CMD_EXPORT(test_media_url, test media url)

void test_media_pause() {
    AudioService.pause_media();
}

MSH_CMD_EXPORT(test_media_pause, test media url)

void test_media_stop() {
    AudioService.stop_media();
}

MSH_CMD_EXPORT(test_media_stop, test media url)

void test_media_play() {
    AudioService.play_media();
}

MSH_CMD_EXPORT(test_media_play, test media url)

void test_raw_event(PlayStateEvent event) {
    if (event == PLAY_EVENT_FINISH) {

    }
    rt_kprintf("=======================test_raw_event  !!!!! >> Event is %d << !!!!=======================\n", event);
}

void test_raw_close(AudioSourcePtr ptr, const size_t id) {
    if (ptr->from == AUDIO_SOURCE_FROM_ONLINE) {
        rt_free(ptr->provider.url);
    }
    typedef void (*Fn)();
    if (ptr->action != NULL) {
        typedef void (*fn)();
        ((Fn) (ptr->action))();
    }
    rt_free(ptr);
}

void test_raw() {
    AudioSourcePtr ptr = rt_malloc(sizeof(AudioSource));
    ptr->priority = AUDIO_SOURCE_PRIORITY_RAW;
    ptr->from = AUDIO_SOURCE_FROM_ONLINE;
    ptr->on_play_event = test_raw_event;
    ptr->provider.url = "/res/piano.wav";
    if (AudioService.media_status() == PLAY_EVENT_START) {
        ptr->action = test_media_play;
    }
    AudioService.post(ptr);
}

MSH_CMD_EXPORT(test_raw, test_raw)

