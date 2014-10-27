/*
 * cc_configfile.c
 *
 *  Created on: 2014年10月27日
 *      Author: demiaowu
 *      Email : cfreestar@163.com
 */
#include "cc_settings.h"
#include "cc_configfile.h"

static int tokenizer_init(tokenizer_t *t, size_t start, size_t size)
{
	t->input	= start;
	t->line 	= 0;
	t->line_pos	= 0;
	t->offset 	= 0;
	t->size		= size;

	return 0;
}

int config_tokenizer(cc_log_t *p_cc_log, tokenizer_t *t)
{
	int ret = 0;

	static config_values_t cv[] = {
			{"cclog_mode", NULL, T_CONFIG_INT, T_CONFIG_GLOBAL},
			{"cclog_path", NULL, T_CONFIG_STRING, T_CONFIG_GLOBAL}
	};

	p_cc_log->cv = cv;

	return ret;
}

int config_prase(cc_log_t *p_cc_log, tokenizer_t *t)
{
	int ret = 0;

	ret = config_tokenizer(p_cc_log, t);

	return ret;
}

int cc_config_parse_file(cc_log_t *p_cc_log, const char * fn)
{
	tokenizer_t t;
	cc_stream_t s;
	int ret;

	if (0 != cc_stream_open(&s, fn)) {
		if (0 == s.size)
			ret = 0;
		else
			ret = -1;
	} else {
		tokenizer_init(&t, s.start, s.size);
		ret = config_prase(p_cc_log, &t);
	}

	cc_stream_close(&s);

	return ret;

}


int cc_config_read(cc_log_t *p_cc_log, const char fn)
{
	cc_config_prase_file(cc_log_t *p_cc_log, fn);
}





