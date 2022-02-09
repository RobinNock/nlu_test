/**
 * @file semantic.c
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "semantic.h"

const char SEMANTIC[] = "semantic\0";
const static char SEMANTIC_DOMAIN[] = "domain\0";
const static char SEMANTIC_INTENT[] = "intent\0";
const static char SEMANTIC_SESSION_COMPLETE[] = "sessionComplete\0";
const static char SEMANTIC_SKILL[] = "skill\0";

inline static char *semantic_parse_domain_from_cjson(const cJSON *cjson)
{
    char *domain = copy_valuestring_from_cjson(cjson);
    return domain;
}

inline static char *semantic_parse_intent_from_cjson(const cJSON *cjson)
{
    char *intent = copy_valuestring_from_cjson(cjson);
    return intent;
}

inline static rt_bool_t semantic_parse_session_complete_from_cjson(const cJSON *cjson)
{
    rt_bool_t session_complete = copy_valuebool_from_cjson(cjson);
    return session_complete;
}

inline static char *semantic_parse_skill_from_cjson(const cJSON *cjosn)
{

    char *skill = copy_valuestring_from_cjson(cjosn);
    return skill;
}

/**
 * @brief 如果cjosn为null，则domain、intent、skill为null，session_complete为false
 *
 * @param cjson
 * @return Semantic
 */
static Semantic semantic_from_cjson(const cJSON *cjson)
{
    const cJSON *domain_cjson = cJSON_GetObjectItem(cjson, SEMANTIC_DOMAIN);
    const cJSON *intent_cjson = cJSON_GetObjectItem(cjson, SEMANTIC_INTENT);
    const cJSON *session_complete_cjson = cJSON_GetObjectItem(cjson, SEMANTIC_SESSION_COMPLETE);
    const cJSON *skill_cjson = cJSON_GetObjectItem(cjson, SEMANTIC_SKILL);

    const Semantic semantic = {
        .domain = semantic_parse_domain_from_cjson(domain_cjson),
        .intent = semantic_parse_intent_from_cjson(intent_cjson),
        .session_complete = semantic_parse_session_complete_from_cjson(session_complete_cjson),
        .skill = semantic_parse_skill_from_cjson(skill_cjson),
    };
    return semantic;
}

/**
 * @brief 如果cjson为null，则返回null
 *
 * @param cjson
 * @return SemanticCPtr
 */
static SemanticCPtr semantic_from_cjson_to_cptr(const cJSON *cjson)
{
    if (cjson == RT_NULL || cJSON_IsNull(cjson))
    {
        return RT_NULL;
    }
    const cJSON *domain_cjson = cJSON_GetObjectItem(cjson, SEMANTIC_DOMAIN);
    const cJSON *intent_cjson = cJSON_GetObjectItem(cjson, SEMANTIC_INTENT);
    const cJSON *session_complete_cjson = cJSON_GetObjectItem(cjson, SEMANTIC_SESSION_COMPLETE);
    const cJSON *skill_cjson = cJSON_GetObjectItem(cjson, SEMANTIC_SKILL);

    SemanticCPtr semantic_ptr = (SemanticPtr)rt_malloc(sizeof(Semantic));
    semantic_ptr->domain = semantic_parse_domain_from_cjson(domain_cjson);
    semantic_ptr->intent = semantic_parse_intent_from_cjson(intent_cjson);
    semantic_ptr->session_complete = semantic_parse_session_complete_from_cjson(session_complete_cjson);
    semantic_ptr->skill = semantic_parse_skill_from_cjson(skill_cjson);

    return semantic_ptr;
}

static void semantic_drop_memory(const SemanticPtr semantic_ptr)
{
    if (semantic_ptr != RT_NULL)
    {
        rt_free(semantic_ptr->domain);
        rt_free(semantic_ptr->intent);
        rt_free(semantic_ptr->skill);
    }
}

static void semantic_drop_memory_self(const SemanticPtr semantic_ptr)
{
    semantic_drop_memory(semantic_ptr);
    rt_free(semantic_ptr);
}

static void semantic_println(const SemanticPtr semantic_ptr)
{
    if (semantic_ptr == RT_NULL)
    {
        return;
    }
    rt_kprintf("tag::semantic\n");
    rt_kprintf("\tdomain:%s\n", semantic_ptr->domain);
    rt_kprintf("\tintent:%s\n", semantic_ptr->intent);
    rt_kprintf("\tsession_complete:%d\n", semantic_ptr->session_complete);
    rt_kprintf("\tskill:%s\n", semantic_ptr->skill);
    rt_kprintf("\n");
}

const _SemanticManager SemanticManager = {
    .from_cjson = semantic_from_cjson,
    .from_cjson_to_cptr = semantic_from_cjson_to_cptr,
    .drop_memory = semantic_drop_memory,
    .drop_memory_and_self = semantic_drop_memory_self,
    .println = semantic_println,
};