#include <stdio.h>
#include <string.h>
#include "shttpd.h"

int uri_decode(char *src, int src_len, char *dst, int dst_len);

static int Method_DoGet(struct worker_ctrl *wctl)
{
	printf("==>Method_DoGet\n");
	struct conn_response *res = &wctl->
}
void Method_Do(struct worker_ctrl *wctl)
{
	printf("==> Method_Do\n");
	switch(wctl->conn.con_req.method)
	{
		case METHOD_GET:
		     Method_DoGet(wctl);
		     break;
		 default:
		      printf("other method\n");     
	}
	printf("<== Method_Do\n");
}
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
    struct  conn_request  req = wctl->conn.con_req; 
    char *pos = wctl->conn.dreq;
	for (m = &_shttpd_methods[0]; m->ptr != NULL; m++)
	{
		if(!strncmp(m->ptr, pos, m->len))
		{
			req.method = m->type;
			break;
		}
	}
	sscanf(wctl->conn.dreq, "%*s %s HTTP/%lu.%lu", req.uri, &req.major, &req.minor);
	printf("url: %s;   major:%lu;    minor:%lu\n", req.uri, req.major, req.minor);

	return NULL;
}
