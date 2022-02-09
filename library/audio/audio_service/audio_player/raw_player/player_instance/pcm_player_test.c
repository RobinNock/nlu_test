//
// Created by 202106 on 2021/11/29.
//



#include <finsh.h>
#include "pcm_player.h"
#include "wav_hdr.h"

void test_pcm_play_event(PcmPlayEvent e) {
    rt_kprintf("play event is %d!!!!!!\n", e);
}

void test_play_wav_file() {
    const char *file_name = "/res/piano.wav";

    FILE *fp = fopen(file_name, "rb");

    if (fp == NULL) {
        rt_kprintf("open file failed!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        return;
    }

    struct wav_header wav;
    wavheader_read(&wav, fp);
    pcm_player_config(wav.fmt_sample_rate, wav.fmt_channels, wav.fmt_bit_per_sample);
    pcm_player_register_on_play_event(test_pcm_play_event);
    pcm_player_play();
    while (1) {
        AllocBytes allocated = pcm_player_alloc_bytes();
        rt_kprintf("write data %d!!!!!!\n", allocated);
        if (allocated.flag == ALLOC_BYTES_FLAG_OK) {
            uint32_t i = fread(allocated.bytes, allocated.size, 1, fp);
            rt_kprintf("write data !!!!!!\n");
            if (i <= 0) {
                allocated.flag = ALLOC_BYTES_LAST_LAST;
                pcm_player_write_bytes(allocated);
                break;
            }
            pcm_player_write_bytes(allocated);
        }
    }
}

MSH_CMD_EXPORT(test_play_wav_file, test_play_wav_file)


void pcm_player_init_test() {
    pcm_player_init();
}

MSH_CMD_EXPORT(pcm_player_init_test, pcm_player_init_test)




