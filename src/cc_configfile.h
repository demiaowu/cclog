/*
 * cc_configfile.h
 *
 *  Created on: 2014年10月27日
 *      Author: demiaowu
 *      Email : cfreestar@163.com
 */

#ifndef CC_CONFIGFILE_H_
#define CC_CONFIGFILE_H_

typedef enum {
	T_CONFIG_UNSET,
	T_CONFIG_STRING,
	T_CONFIG_INT
}config_values_type_t;

typedef enum {
	T_CONFIG_UNSET,
	T_CONFIG_GLOBAL
}config_scope_type_t;


typedef struct {
	const char * key;
	void * destination;

	config_values_type_t type;
	config_scope_type_t scope;
}config_values_t;

/*分词器*/
typedef struct {
	const char *input;
	size_t offset;
	size_t size;

	int line_pos;
	int line;
}tokenizer_t;

int tokenizer_open(cc_log_t *p_cc_log, tokenizer_t *t, const char *fn);


#endif /* CC_CONFIGFILE_H_ */
