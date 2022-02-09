/**
 * @file music.c
 * @author qr-kou (codinlog@foxmail.com)
 * @brief 音乐
 * @version 0.1
 * @date 2021-10-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "music.h"

const static char MUSIC_URL[] = "url\0";
const static char MUSIC_SINGER[] = "singer\0";
const static char MUSIC_SONG[] = "song\0";
const static char MUSIC_IMAGE[] = "image\0";
const static char MUSIC_ALBUM[] = "album\0";
const static char MUSIC_SONG_ID[] = "songId";

inline static char *music_parse_url_from_cjson(const cJSON *cjson)
{
    char *url = copy_valuestring_from_cjson(cjson);
    return url;
}

inline static char *music_parse_singer_from_cjson(const cJSON *cjson)
{
    char *singer = copy_valuestring_from_cjson(cjson);
    return singer;
}

inline static char *music_parse_song_from_cjson(const cJSON *cjson)
{
    char *song = copy_valuestring_from_cjson(cjson);
    return song;
}

inline static char *music_parse_image_from_cjson(const cJSON *cjson)
{
    char *image = copy_valuestring_from_cjson(cjson);
    return image;
}

inline static char *music_parse_album_from_cjson(const cJSON *cjson)
{
    char *album = copy_valuestring_from_cjson(cjson);
    return album;
}

inline static char *music_parse_song_id_from_cjson(const cJSON *cjson)
{
    char *song_id = copy_valuestring_from_cjson(cjson);
    return song_id;
}

static Music music_from_cjson(const cJSON *cjson)
{
    const cJSON *url_cjson = cJSON_GetObjectItem(cjson, MUSIC_URL);
    const cJSON *singer_cjson = cJSON_GetObjectItem(cjson, MUSIC_SINGER);
    const cJSON *song_cjson = cJSON_GetObjectItem(cjson, MUSIC_SONG);
    const cJSON *image_cjson = cJSON_GetObjectItem(cjson, MUSIC_IMAGE);
    const cJSON *album_cjson = cJSON_GetObjectItem(cjson, MUSIC_ALBUM);
    const cJSON *song_id_cjson = cJSON_GetObjectItem(cjson, MUSIC_SONG_ID);

    Music music = {
        .url = music_parse_url_from_cjson(url_cjson),
        .singer = music_parse_singer_from_cjson(singer_cjson),
        .song = music_parse_song_from_cjson(song_cjson),
        .image = music_parse_image_from_cjson(image_cjson),
        .album = music_parse_album_from_cjson(album_cjson),
        .song_id = music_parse_song_id_from_cjson(song_id_cjson),
    };
    return music;
}

static MusicCPtr music_from_cjson_to_cptr(const cJSON *cjson)
{
    if (cjson == RT_NULL)
    {
        return RT_NULL;
    }

    const cJSON *url_cjson = cJSON_GetObjectItem(cjson, MUSIC_URL);
    const cJSON *singer_cjson = cJSON_GetObjectItem(cjson, MUSIC_SINGER);
    const cJSON *song_cjson = cJSON_GetObjectItem(cjson, MUSIC_SONG);
    const cJSON *image_cjson = cJSON_GetObjectItem(cjson, MUSIC_IMAGE);
    const cJSON *album_cjson = cJSON_GetObjectItem(cjson, MUSIC_ALBUM);
    const cJSON *song_id_cjson = cJSON_GetObjectItem(cjson, MUSIC_SONG_ID);

    MusicPtr music_ptr = (MusicCPtr)rt_malloc(sizeof(Music));
    music_ptr->url = music_parse_url_from_cjson(url_cjson);
    music_ptr->singer = music_parse_singer_from_cjson(singer_cjson);
    music_ptr->song = music_parse_song_from_cjson(song_cjson);
    music_ptr->image = music_parse_image_from_cjson(image_cjson);
    music_ptr->album = music_parse_album_from_cjson(album_cjson);
    music_ptr->song_id = music_parse_song_id_from_cjson(song_id_cjson);
    return music_ptr;
}

static void music_drop_memory(const MusicPtr music_ptr)
{
    if (music_ptr != RT_NULL)
    {
        rt_free(music_ptr->url);
        rt_free(music_ptr->singer);
        rt_free(music_ptr->song);
        rt_free(music_ptr->image);
        rt_free(music_ptr->album);
        rt_free(music_ptr->song_id);
    }
}

static void music_drop_memory_and_self(const MusicPtr music_ptr)
{
    music_drop_memory(music_ptr);
    rt_free(music_ptr);
}

static void music_println(const MusicPtr music_ptr)
{
    if (music_ptr == RT_NULL)
    {
        return;
    }
    rt_kprintf("tag::music\n");
    rt_kprintf("\turl:%s\n", music_ptr->url);
    rt_kprintf("\tsinger:%s\n", music_ptr->singer);
    rt_kprintf("\tsong:%s\n", music_ptr->song);
    rt_kprintf("\timage:%s\n", music_ptr->image);
    rt_kprintf("\talbum:%s\n", music_ptr->album);
    rt_kprintf("\tsong_id:%s\n", music_ptr->song_id);
    rt_kprintf("\n");
}

const _MusicManager MusicManager = {
    .from_cjson = music_from_cjson,
    .from_cjson_to_cptr = music_from_cjson_to_cptr,
    .drop_memory = music_drop_memory,
    .drop_memory_and_self = music_drop_memory_and_self,
    .println = music_println,
};