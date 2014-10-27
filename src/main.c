/*
 * main.c
 *
 *  Created on: 2014年10月20日
 *      Author: demiaowu
 *      Email : cfreestar@163.com
 */
#include "cc_log.h"

int main(int argc ,char *argv[])
{
	cc_log_t *p_cc_log = cc_log_init_from_conf();

	printf("%d\n", p_cc_log->log_mode);

	cc_log_write_caf_t(p_cc_log,
			"s",
			"nihao");

	return 0;
}




