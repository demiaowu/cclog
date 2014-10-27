/*
 * cc_log.c
 *
 *  Created on: 2014-10-20日
 *      Author: demiaowu
 *      Email : cfreestar@163.com
 */

#include "cc_log.h"


cc_log_t* cc_log_init()
{
	cc_log_t *p_cc_log;

	p_cc_log = malloc(sizeof(*p_cc_log));
	assert(p_cc_log);

	p_cc_log->log_fd 	= -1;
	p_cc_log->log_mode 	= CC_LOG_STDOUT;
	p_cc_log->log_str 	= NULL;

	return p_cc_log;
}


/*Close the cc_log_fd and try to get a /dev/null/ for it instead.
 *
 *Return 0 on success and -1 for failure.
 */
int open_dev_null(int log_fd)
{
	int tmpfd;

	if (close(log_fd) != 0)
		return -1;

	tmpfd = open("/dev/null", O_RDWR);

	if (tmpfd!=-1 && tmpfd!=log_fd)
	{
		dup2(tmpfd, log_fd);
		/*Close the temporary file descriptor, avoid the waste of
		 * resources.
		 */
		close(tmpfd);
	}

	return (tmpfd!=-1) ? 0 : -1;
}
/*Init cclog form the configuration: cclog.conf
 * */
cc_log_t* cc_log_init_from_conf()
{

	FILE *p_logconf = NULL;
	int close_stdout_or_stderr = 1;
	cc_log_t *p_cc_log = cc_log_init();

	p_logconf = fopen("cclog.conf", "r+");
	if (p_logconf != NULL)
	{
		/*get the log mode and the */
	}
	else
	{
		fprintf(stderr, "%s %s %d fopen('cclog.conf', 'r+') failed\n",
					__FILE__,
					__FUNCTION__,
					__LINE__);
		close_stdout_or_stderr = 0;
	}
#ifdef HAVE_VALGRIND_VALGRIND_H
	if (RUNNING_ON_VALGRIND)
		close_stdout_or_strerr = 0;
#endif

#if 0
	if ((p_cc_log->log_mode == CC_LOG_STDOUT ||
			p_cc_log->log_mode == CC_LOG_STDERR ) &&
			不是守护进程)
		close_stdout_or_stderr = 0;
#endif

	if (close_stdout_or_stderr) {
		open_dev_null(STDERR_FILENO);
		open_dev_null(STDOUT_FILENO);
	}

	return p_cc_log;
}

int cc_log_open(cc_log_t *p_cc_log)
{
	switch (p_cc_log->log_mode)
	{
	case CC_LOG_STDOUT:
	case CC_LOG_STDERR:
		break;
	case CC_LOG_SYSLOG:
#ifdef HEVE_SYSLOG_H
		closelog();
#endif
		break;
	case CC_LOG_FILE:
		close(p_cc_log->log_fd);
		break;
	}

	return 0;
}

int cc_log_write(cc_log_t *p_cc_log, const char *filename, unsigned int line, const char *fmt, ...)
{
	time_t t;
	va_list ap;
	cc_log_str_t *p_log_str;

	p_log_str = cc_log_str_init();
	cc_log_str_prepare_append(p_log_str, 256);

	switch(p_cc_log->log_mode)
	{
	case CC_LOG_STDOUT:
	case CC_LOG_STDERR:
	case CC_LOG_FILE:
		strftime(p_log_str->p_str, p_log_str->size-1, "%Y-%m-%d %H:%M:%S", localtime(&t));
		p_log_str->used  = strlen(p_log_str->p_str) + 1;
		cc_log_str_append_str(p_log_str, ": (");
		break;
		/*syslog is gernerating its own timestamps.*/
	case CC_LOG_SYSLOG:
		cc_log_str_append_str(p_log_str, " (");
		break;
	}

	cc_log_str_append_str(p_log_str, filename);
	cc_log_str_append_str(p_log_str, ".");
	cc_log_str_append_long(p_log_str, line);
	cc_log_str_append_str(p_log_str, ") ");

	for (va_start(ap, fmt); *fmt; fmt++)
	{
		int 	i;
		long 	l;
		float	f;
		double 	d;
		char 	*s;
		char 	c;


		switch (*fmt)
		{
		case 's':
			s = va_arg(ap, char*);
			cc_log_str_append_str(p_log_str, s);
			cc_log_str_append_str(p_log_str, " ");
			break;
		case 'i':
			i = va_arg(ap, int);
			cc_log_str_append_int(p_log_str, i);
			cc_log_str_append_str(p_log_str, " ");
			break;
		case 'l':
			l = va_arg(ap, long);
			cc_log_str_append_long(p_log_str, l);
			cc_log_str_append_str(p_log_str, " ");
			break;
		case 'f':
			/*注意，我们用va_arg(ap,type)取出一个参数的时候，
				type绝对不能为以下类型：
				——char、signed char、unsigned char
				——short、unsigned short
				——signed short、short int、signed short int、unsigned short int
				——float

				注意下面float和char的处理技巧
			 * */
			f = (float)va_arg(ap, double);
			cc_log_str_append_float(p_log_str, f);
			cc_log_str_append_str(p_log_str, " ");
			break;
		case 'd':
			d = va_arg(ap, double);
			cc_log_str_append_double(p_log_str, d);
			cc_log_str_append_str(p_log_str, " ");
			break;
		case 'c':
			c = (char)va_arg(ap, int);
			cc_log_str_append_char(p_log_str, c);
			cc_log_str_append_str(p_log_str, " ");
			break;
#if 0
		default:
			 cc_log_str_append_str_top_len(p_log_str, fmt, 1);
#endif
		}
	}
	va_end(ap);

	switch (p_cc_log->log_mode)
	{
	case CC_LOG_STDOUT:
		write(STDOUT_FILENO, p_log_str->p_str, p_log_str->used-1);
		break;
	case CC_LOG_STDERR:
		write(STDERR_FILENO, p_log_str->p_str, p_log_str->used-1);
		break;
	case CC_LOG_SYSLOG:
#ifdef HAVE_SYSLOG_H
		syslog(LOG_ERR, "%s", p_log_str->p_log_str);
#endif
		break;
	case CC_LOG_FILE:
		write(p_cc_log->log_fd, p_log_str->p_str, p_log_str->used-1);
		break;
	}

	cc_log_str_free(p_log_str);

	return 0;
}

int cc_log_write_va_list(cc_log_t *p_cc_log, const char *filename, unsigned int line, const char *fmt, va_list ap)
{
	cc_log_str_t *p_log_str;

	p_log_str = cc_log_str_init();
	cc_log_str_prepare_append(p_log_str, 256);

	switch(p_cc_log->log_mode)
	{
		case CC_LOG_STDOUT:
	case CC_LOG_STDERR:
	case CC_LOG_FILE:
		strftime(p_log_str->p_str, p_log_str->size-1, "%Y-%m-%d %H:%M:%S", localtime(NULL));
		p_log_str->used  = strlen(p_log_str->p_str) + 1;
		cc_log_str_append_str(p_log_str, ": (");
		break;
		/*syslog is gernerating its own timestamps.*/
	case CC_LOG_SYSLOG:
		cc_log_str_append_str(p_log_str, " (");
		break;
	}

	cc_log_str_append_str(p_log_str, filename);
	cc_log_str_append_str(p_log_str, ".");
	cc_log_str_append_long(p_log_str, line);
	cc_log_str_append_str(p_log_str, ") ");

	for ( ; *fmt; fmt++)
	{
		int 	i;
		long 	l;
		float	f;
		double 	d;
		char 	*s;
		char 	c;


		switch (*fmt)
		{
		case 's':
			s = va_arg(ap, char*);
			cc_log_str_append_str(p_log_str, s);
			cc_log_str_append_str(p_log_str, " ");
			break;
		case 'i':
			i = va_arg(ap, int);
			cc_log_str_append_int(p_log_str, i);
			cc_log_str_append_str(p_log_str, " ");
			break;
		case 'l':
			l = va_arg(ap, long);
			cc_log_str_append_long(p_log_str, l);
			cc_log_str_append_str(p_log_str, " ");
			break;
		case 'f':
		/*注意，我们用va_arg(ap,type)取出一个参数的时候，
			type绝对不能为以下类型：
			——char、signed char、unsigned char
			——short、unsigned short
			——signed short、short int、signed short int、unsigned short int
			——float

			注意下面float和char的处理技巧
		 * */
			f = (float)va_arg(ap, double);
			cc_log_str_append_float(p_log_str, f);
			cc_log_str_append_str(p_log_str, " ");
			break;
		case 'd':
			d = va_arg(ap, double);
			cc_log_str_append_double(p_log_str, d);
			cc_log_str_append_str(p_log_str, " ");
			break;
		case 'c':
			c = (char)va_arg(ap, int);
			cc_log_str_append_char(p_log_str, c);
			cc_log_str_append_str(p_log_str, " ");
			break;
		default:
			 cc_log_str_append_str_top_len(p_log_str, fmt, 1);
		}
	}

	switch (p_cc_log->log_mode)
	{
	case CC_LOG_STDOUT:
		write(STDOUT_FILENO, p_log_str->p_str, p_log_str->used-1);
		break;
	case CC_LOG_STDERR:
		write(STDERR_FILENO, p_log_str->p_str, p_log_str->used-1);
		break;
	case CC_LOG_SYSLOG:
#ifdef HAVE_SYSLOG_H
		syslog(LOG_ERR, "%s", p_log_str->p_log_str);
#endif
		break;
	case CC_LOG_FILE:
		write(p_cc_log->log_fd, p_log_str->p_str, p_log_str->used-1);
		break;
	}

	cc_log_str_free(p_log_str);

	return 0;
}

/*注意这样产生__FILE__, __LINE__始终是cc_log.c 334行，显然是不行的*/
int cc_log_write_caf(cc_log_t *p_cc_log, const char* ftm, ...)
{
	va_list ap;
	int 	r;
	va_start(ap, ftm);
	r = cc_log_write(p_cc_log, __FILE__, __LINE__, ftm, ap);
	va_end(ap);

	return r;
}


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

