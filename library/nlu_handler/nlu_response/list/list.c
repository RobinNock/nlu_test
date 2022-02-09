/**
 * @file list.c
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "list.h"

const char LIST[] = "list\0";

static AnyPtr list_at(const ListPtr list_ptr, const size_t poition)
{
    if (poition >= list_ptr->_size)
    {
        return RT_NULL;
    }
    return list_ptr->_item[poition];
}

static AnyPtr list_next(const ListPtr list_ptr)
{

    return (list_ptr->_item[list_ptr->_cursor++]);
}

static AnyPtr list_prev(const ListPtr list_ptr)
{
    return (list_ptr->_item[list_ptr->_cursor--]);
}

static rt_bool_t list_has_next(const ListPtr list_ptr)
{
    return list_ptr->_size > list_ptr->_cursor;
}

static rt_bool_t list_has_prev(const ListPtr list_ptr)
{
    return list_ptr->_cursor < 0;
}

static void list_reiterate(const ListPtr list_ptr)
{
    list_ptr->_cursor = 0;
}

static void
list_foreach(const ListPtr list_ptr, void (*callback)(const AnyPtr item))
{
    for (size_t i = 0; i < list_ptr->_size; i++)
    {
        const AnyPtr item = list_ptr->_item[i];
        callback(item);
    }
}

inline static MusicCPtr list_music_parse_from_cjson_to_cptr(const cJSON *cjson)
{
    return MusicManager.from_cjson_to_cptr(cjson);
}

inline static JokeCPtr list_joke_parse_from_cjson_to_cptr(const cJSON *cjson)
{
    return JokeManager.from_cjson_to_cptr(cjson);
}

inline static PoemCPtr list_poem_parse_from_cjson_to_cptr(const cJSON *cjson)
{
    return PoemManager.from_cjson_to_ptr(cjson);
}

inline static RadioCPtr list_radio_parse_from_cjson_to_cptr(const cJSON *cjson)
{
    return RadioManager.from_cjson_to_cptr(cjson);
}

// static List list_music_from_cjson(const cJSON *cjson)
// {
//     List list = {
//         ._size = 0,
//         ._cursor = 0,
//         ._item = RT_NULL,
//         .type = MUSIC,
//     };
//     if (!cJSON_IsArray(cjson))
//     {
//         return list;
//     }

//     size_t size = cJSON_GetArraySize(cjson);
//     MusicPtr *music_ptr = (MusicPtr *)rt_malloc(size * sizeof(MusicPtr));
//     for (size_t i = 0; i < size; i++)
//     {
//         const cJSON *music_cjson = cJSON_GetArrayItem(cjson, i);
//         music_ptr[i] = list_music_parse_from_cjson_to_cptr(music_cjson);
//     }
//     list._size = size;
//     list._item = music_ptr;
//     return list;
// }

static ListCPtr list_music_from_cjson_to_cptr(const cJSON *cjson)
{
    const size_t size = cJSON_GetArraySize(cjson);
    MusicPtr *music_ptr = (MusicPtr *)rt_malloc(size * sizeof(MusicPtr));
    for (size_t i = 0; i < size; i++)
    {
        const cJSON *music_cjson = cJSON_GetArrayItem(cjson, i);
        music_ptr[i] = list_music_parse_from_cjson_to_cptr(music_cjson);
    }

    ListPtr list_ptr = (ListPtr)rt_malloc(sizeof(List));
    list_ptr->_size = size;
    list_ptr->_cursor = 0;
    list_ptr->_item = music_ptr;
    list_ptr->type = MUSIC;

    return list_ptr;
}

static ListCPtr list_joke_from_cjson_to_cptr(const cJSON *cjson)
{
    const size_t size = cJSON_GetArraySize(cjson);
    JokePtr *joke_ptr = (JokePtr *)rt_malloc(size * sizeof(JokePtr));
    for (size_t i = 0; i < size; i++)
    {
        const cJSON *joke_cjson = cJSON_GetArrayItem(cjson, i);
        joke_ptr[i] = list_joke_parse_from_cjson_to_cptr(joke_cjson);
    }

    ListPtr list_ptr = (ListPtr)rt_malloc(sizeof(List));
    list_ptr->_size = size;
    list_ptr->_cursor = 0;
    list_ptr->_item = joke_ptr;
    list_ptr->type = JOKE;

    return list_ptr;
}

static ListCPtr list_poem_from_cjson_to_cptr(const cJSON *cjson)
{
    const size_t size = cJSON_GetArraySize(cjson);
    PoemPtr *poem_ptr = (PoemPtr *)rt_malloc(size * sizeof(PoemPtr));
    for (size_t i = 0; i < size; i++)
    {
        const cJSON *poem_cjson = cJSON_GetArrayItem(cjson, i);
        poem_ptr[i] = list_poem_parse_from_cjson_to_cptr(poem_cjson);
    }

    ListPtr list_ptr = (ListPtr)rt_malloc(sizeof(List));
    list_ptr->_size = size;
    list_ptr->_cursor = 0;
    list_ptr->_item = poem_ptr;
    list_ptr->type = POEM;

    return list_ptr;
}

static ListCPtr list_radio_from_cjson_to_cptr(const cJSON *cjson)
{
    const size_t size = cJSON_GetArraySize(cjson);
    RadioPtr *radio_ptr = (RadioPtr *)rt_malloc(size * sizeof(RadioPtr));
    for (size_t i = 0; i < size; i++)
    {
        const cJSON *radio_cjson = cJSON_GetArrayItem(cjson, i);
        radio_ptr[i] = list_radio_parse_from_cjson_to_cptr(radio_cjson);
    }

    ListPtr list_ptr = (ListPtr)rt_malloc(sizeof(List));
    list_ptr->_size = size;
    list_ptr->_cursor = 0;
    list_ptr->_item = radio_ptr;
    list_ptr->type = RADIO;

    return list_ptr;
}

static ListCPtr list_from_cjson_to_cptr(const cJSON *cjson, DomainTypeEnum type)
{
    if (cjson == RT_NULL)
    {
        return RT_NULL;
    }

    if (cJSON_IsNull(cjson))
    {
        return RT_NULL;
    }

    if (!cJSON_IsArray(cjson))
    {
        return RT_NULL;
    }

    if (type == MUSIC)
    {

        return list_music_from_cjson_to_cptr(cjson);
    }

    if (type == RADIO)
    {
        return list_radio_from_cjson_to_cptr(cjson);
    }

    if (type == POEM)
    {
        return list_poem_from_cjson_to_cptr(cjson);
    }

    if (type == JOKE)
    {
        return list_joke_from_cjson_to_cptr(cjson);
    }

    return RT_NULL;
}

static void list_music_drop_memory_entry(const AnyPtr item)
{
    const MusicPtr music_ptr = (MusicPtr)item;
    MusicManager.drop_memory_and_self(music_ptr);
}

static void list_joke_drop_memory_entry(const AnyPtr item)
{
    const JokePtr joke_ptr = (JokePtr)item;
    JokeManager.drop_memory_and_self(joke_ptr);
}

static void list_poem_drop_memory_entry(const AnyPtr item)
{
    const PoemPtr poem_ptr = (PoemPtr)item;
    PoemManager.drop_memory_and_self(poem_ptr);
}

static void list_radio_drop_memory_entry(const AnyPtr item)
{
    const RadioPtr radio_ptr = (RadioPtr)item;
    RadioManager.drop_memory_and_self(radio_ptr);
}

static void list_drop_memory(const ListPtr list_ptr)
{
    if (list_ptr != RT_NULL)
    {
        if (list_ptr->type == MUSIC)
        {
            list_foreach(list_ptr, list_music_drop_memory_entry);
        }

        if (list_ptr->type == JOKE)
        {
            list_foreach(list_ptr, list_joke_drop_memory_entry);
        }

        if (list_ptr->type == RADIO)
        {
            list_foreach(list_ptr, list_radio_drop_memory_entry);
        }

        if (list_ptr->type == POEM)
        {
            list_foreach(list_ptr, list_poem_drop_memory_entry);
        }

        rt_free(list_ptr->_item);
    }
}

static void list_drop_memory_and_self(const ListPtr list_ptr)
{
    list_drop_memory(list_ptr);
    rt_free(list_ptr);
}

static void list_println_music_entry(const AnyPtr item)
{
    const MusicPtr music_ptr = (MusicPtr)item;
    MusicManager.println(music_ptr);
}

static void list_println_joke_entry(const AnyPtr item)
{
    const JokePtr joke_ptr = (JokePtr)item;
    JokeManager.println(joke_ptr);
}

static void list_println_radio_entry(const AnyPtr item)
{
    const RadioPtr radio_ptr = (RadioPtr)item;
    RadioManager.println(radio_ptr);
}

static void list_println_poem_entry(const AnyPtr item)
{
    const PoemPtr poem_ptr = (PoemPtr)item;
    PoemManager.println(poem_ptr);
}

static void list_println(const ListPtr list_ptr)
{
    if (list_ptr == RT_NULL)
    {
        return;
    }
    rt_kprintf("tag::list\n");
    switch (list_ptr->type)
    {
    case MUSIC:
    {
        ListManager.foreach (list_ptr, list_println_music_entry);
    }
    break;
    case JOKE:
    {
        ListManager.foreach (list_ptr, list_println_joke_entry);
    }
    break;
    case RADIO:
    {
        ListManager.foreach (list_ptr, list_println_radio_entry);
    }
    break;
    case POEM:
    {
        ListManager.foreach (list_ptr, list_println_poem_entry);
    }
    break;
    }

    rt_kprintf("\n");
}

const _ListManager ListManager = {
    .from_cjson_to_cptr = list_from_cjson_to_cptr,
    .has_next = list_has_next,
    .has_prev = list_has_prev,
    .next = list_next,
    .prev = list_prev,
    .reiterate = list_reiterate,
    .at = list_at,
    .foreach = list_foreach,
    .drop_memory = list_drop_memory,
    .drop_memory_and_self = list_drop_memory_and_self,
    .println = list_println,
};
