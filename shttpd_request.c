#include <stdio.h>
#include <string.h>
#include "shttpd.h"


char *request_parser( struct worker_ctrl *wctl)
{
	printf("%s\n", wctl->conn.dreq); 
// 	GET /lhd HTTP/1.1
// Host: localhost:8080
// User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:42.0) Gecko/20100101 Firefox/42.0
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
// Accept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3
// Accept-Encoding: gzip, deflate
    vec *m = NULL;
    char *pos = wctl->conn.dreq;
	for (m = &_shttpd_methods[0]; m->ptr != NULL; m++)
	{
		if(!strncmp(m->ptr, pos, m->len))
		{
			wctl->conn.method = m->type;
		}
	}
	sscanf(wctl->conn.dreq, "%[A-Z] %s HTTP/%lu.%lu", method, uri, major, minor);
	return NULL;
}
