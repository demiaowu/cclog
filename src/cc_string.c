/*
 * cc_string.c
 *
 *  Created on: 2014年10月27日
 *      Author: demiaowu
 *      Email : cfreestar@163.co
 */
#include "cc_string.h"

cc_log_str_t* cc_log_str_init()
{
	cc_log_str_t *p_log_str;

	p_log_str = malloc(sizeof(*p_log_str));
	assert(p_log_str);

	p_log_str->p_str	= NULL;
	p_log_str->used 	= 0;
	p_log_str->size		= 0;

	return p_log_str;
}

void cc_log_str_free(cc_log_str_t *p_log_str)
{
	if (!p_log_str)
		return;
	free(p_log_str->p_str);
	free(p_log_str);
}

/*Used data will be lost*/
int cc_log_str_prepare_copy(cc_log_str_t *p_log_str, size_t size)
{
	if (!p_log_str)
		return -1;

	if ( (0 == p_log_str->size)
			|| (size > p_log_str->size))
	{
		if (p_log_str->size)
			free(p_log_str->p_str);

		p_log_str->size = size;
		p_log_str->size += CC_LOG_STR_PIECE_SIZE - (p_log_str->size%CC_LOG_STR_PIECE_SIZE);

		p_log_str->p_str = malloc(p_log_str->size);
	}

	p_log_str->used = 0;

	return 0;
}

/* increase the internal cc_log_str_t (if neccessary) to append another 'size' byte
 * ->used isn't changed
 */
int cc_log_str_prepare_append(cc_log_str_t *p_log_str, size_t size)
{
	if (!p_log_str)
		return -1;

	if (0 == p_log_str->size)
	{
		p_log_str->size = size;
		p_log_str->size += CC_LOG_STR_PIECE_SIZE - (p_log_str->size%CC_LOG_STR_PIECE_SIZE);

		p_log_str->p_str = malloc(p_log_str->size);
		assert(p_log_str->p_str);
	}
	else if (p_log_str->used+size > p_log_str->size)
	{
		p_log_str->size += size;
		p_log_str += CC_LOG_STR_PIECE_SIZE - (p_log_str->size%CC_LOG_STR_PIECE_SIZE);

		p_log_str->p_str = realloc(p_log_str->p_str, p_log_str->size);
		assert(p_log_str->p_str);
	}

	return 0;
}

int cc_log_str_append_str(cc_log_str_t *p_log_str, const char *s)
{
	size_t s_len;

	if (!p_log_str || !s)
		return -1;

	s_len = strlen(s);

	/*此处不必去判断原先是否有足够的空间存放，因为在cc_log_str_prepare_append函数内部已经判断了*/
	cc_log_str_prepare_append(p_log_str, s_len+1);
	/*这里很好的处理在p_log_str->p_str是空的情况
	 * 避免了在memcpy时总是正确的*/
	if (p_log_str->used == 0)
		p_log_str->used ++;

	memcpy(p_log_str->p_str + p_log_str->used -1, s, s_len+1);
	p_log_str->used += s_len;

	return 0;
}

int cc_log_str_append_str_top_len(cc_log_str_t *p_log_str, const char *s, size_t s_len)
{
	if (!p_log_str || !s)
		return -1;
	if (s_len == 0)
		return 0;

	cc_log_str_prepare_append(p_log_str, s_len+1);
	if (p_log_str->used == 0)
		p_log_str->used ++;

	memcpy(p_log_str->p_str+p_log_str->used -1, s, s_len);
	p_log_str->used += s_len;
	p_log_str->p_str[p_log_str->used-1] = '\0';

	return 0;
}

int cc_log_str_append_int(cc_log_str_t *p_log_str, int val)
{
	char s_val[32];

	sprintf(s_val, "%d", val);
	cc_log_str_append_str(p_log_str, s_val);

	return 0;
}

int cc_log_str_append_long(cc_log_str_t *p_log_str, long val)
{
	char s_val[32];

	sprintf(s_val, "%ld", val);
	cc_log_str_append_str(p_log_str, s_val);

	return 0;
}

int cc_log_str_append_float(cc_log_str_t *p_log_str, float val)
{
	char s_val[64];

	sprintf(s_val, "%f", val);
	cc_log_str_append_str(p_log_str, s_val);

	return 0;
}

int cc_log_str_append_double(cc_log_str_t *p_log_str, double val)
{
	char s_val[64];

	sprintf(s_val, "%lf", val);
	cc_log_str_append_str(p_log_str, s_val);

	return 0;
}
int cc_log_str_append_char(cc_log_str_t *p_log_str, const char ch)
{
	char s_val[4];
	sprintf(s_val, "%c", ch);
	cc_log_str_append_str(p_log_str, s_val);

	return 0;
}


