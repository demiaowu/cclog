/*
 * cc_stream.h
 *
 *  Created on: 2014年10月27日
 *      Author: demiaowu
 *      Email : cfreestar@163.com
 */

#ifndef CC_STREAM_H_
#define CC_STREAM_H_

#include "cc_settings.h"
#include "cc_string.h"

typedef struct {
	char *start;
	off_t size;
}cc_stream_t;

int cc_stream_open(cc_stream_t *f, cc_log_str_t *fn);

int cc_stream_close(cc_stream_t *f);


#endif /* CC_STREAM_H_ */
