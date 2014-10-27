/*
 * cc_string.h
 *
 *  Created on: 2014年10月27日
 *      Author: demiaowu
 *      Email : cfreestar@163.com
 */
#ifndef CC_STRING_H_
#define CC_STRING_H_

#include "cc_config.h"

#define CC_LOG_STR_PIECE_SIZE 64

/*使用used，size可以方便的实现字符创的清空和重置*/
typedef struct cc_log_str {
	char   *p_str;
	size_t used;		/*the length of used space, include the last char '\0'*/
	size_t size;
}cc_log_str_t;

/*The function of cc_log_str_t */
cc_log_str_t* cc_log_str_init();

void cc_log_str_free(cc_log_str_t *p_log_str);

int cc_log_str_prepare_copy(cc_log_str_t *p_log_str, size_t size);

int cc_log_str_prepare_append(cc_log_str_t *p_log_str, size_t size);

int cc_log_str_append_str(cc_log_str_t *p_log_str, const char *s);

int cc_log_str_append_char(cc_log_str_t *p_log_str, const char ch);

int cc_log_str_append_str_top_len(cc_log_str_t *p_log_str, const char *s, size_t s_len);

int cc_log_str_append_int(cc_log_str_t *p_log_str, int val);

int cc_log_str_append_long(cc_log_str_t *p_log_str, long val);

int cc_log_str_append_float(cc_log_str_t *p_log_str, float val);

int cc_log_str_append_double(cc_log_str_t *p_log_str, double val);


#endif /* CC_STRING_H_ */
