#include <stdio.h>
#include <string.h>
#include "shttpd.h"


char *request_parser( struct worker_ctrl *wctl)
{
	printf("%s\n", wctl->conn.dreq); 
	// wctl.conn
	return NULL;
}
