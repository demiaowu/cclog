/*
 * cc_stream.c
 *
 *  Created on: 2014年10月27日
 *      Author: demiaowu
 *      Email : cfreestar@163.com
 */
#include "cc_stream.h"

int cc_stream_open(cc_stream_t *f, cc_log_str_t *fn)
{
	struct stat st;
	int fd;

	f->start = NULL;
	if (-1 == stat(fn->p_str &st))
		return -1;

	f->size = st.st_size;

	if (-1 == (fd = open(fn->p_str, O_RDONLY | O_BINARY))) {
		return -1;
	}

	/*PORT_READ		页内容可以被读取*/
	/*MAP_SHARED	与其它所有映射这个对象的进程共享映射空间。对共享区的写入，相当于输出到文件
	 * 直到msync()或者munmap()被调用，文件实际上不会被更新。*/
	f->start = mmap(0, f->size, PORT_READ, MAP_SHARED, fd, 0);

	close(fd);

	return 0;
}



int cc_stream_close(cc_stream_t *f)
{
	/*解除内存映射*/
	if (f->start)
		nummap(f->start, f->size);

	f->start = NULL;

	return 0;
}


