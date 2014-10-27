/*
 * cc_log.h
 *
 *  Created on: 2014-10-20
 *      Author: demiaowu
 *      Email : cfreestar@163.com
 */

#ifndef CC_LOG_H_
#define CC_LOG_H_

#include "cc_settings.h"



#define cc_log_write_caf_t(p_cc_log, fmt, ...) \
	cc_log_write(p_cc_log, __FILE__, __LINE__, fmt, __VA_ARGS__);

#if 0
			va_list ap;	\
			va_start(ap, fmt);	\
			cc_log_write(p_cc_log, __FILE__, __LINE__, fmt, ap);	\
			va_end(ap);
#endif



/*log mode*/
typedef enum {
		CC_LOG_STDOUT = 1,	/*stdout*/
		CC_LOG_STDERR,		/*stderr*/
		CC_LOG_SYSLOG,		/*syslog*/
		CC_LOG_FILE			/*file	*/
}cc_log_mode_t;


typedef struct {
	cc_log_mode_t log_mode;		/*cc_log mode*/
	int   log_fd;			/*cc_log file descriptor*/
	char* log_str;			/*the string(char *) of the log*/
	config_values_t *cv;
}cc_log_t;



/*Close the cc_log_fd and try to get a /dev/null/ for it instead.
 *
 *Return 0 on success and -1 for failure.
 */
int open_dev_null(int log_fd);

cc_log_t* cc_log_init();

int cc_log_free(cc_log_t *p_cc_log);

/*Init the cclog by configuration file of cclog.conf,
 * include set the log mode and the file for record the log.
 **/
cc_log_t* cc_log_init_from_conf();


/* Open the log
 *
 * We have 4 possibilities:
 * 		--	stdout(default)
 * 		--	stderr
 * 		-- 	syslog
 * 		--  file
 *
 * Return 0 on success, If the open failed, report to user and return -1,
 * */
int cc_log_open(cc_log_t *cc_log);

/* Close cc_log by it's log mode respectively
 *
 * Return 0 on success and -1 for failure.
 **/
int cc_log_close(cc_log_t *cc_log);

/* Write the log on the designated place which is configure in the cclog.conf file.
 *
 * Return 0 on success and -1 for failure.
 * */
int cc_log_write(cc_log_t *p_cc_log, const char *filename, unsigned int line, const char *fmt, ...);

int cc_log_write_va_list(cc_log_t *p_cc_log, const char *filename, unsigned int line, const char *fmt, va_list ap);

/*The convenient and fast express of cc_log_write();
 * */
int cc_log_write_caf(cc_log_t *p_cc_log, const char* ftm, ...);









#endif /* CC_LOG_H_ */
