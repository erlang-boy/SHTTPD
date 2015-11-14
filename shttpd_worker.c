#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h> 
#include <pthread.h>
#include <stdlib.h>
#include "shttpd.h"
#define BUFFLEN 1024
#define PORT 8080
#define BACK_LOG 10

#define STATUS_RUNNING 0
#define STATUS_STOP    1	
#define WORKER_IDEL    2
#define WORKER_DETACHED 3
#define WORKER_DETACHING 4
int STATUS = STATUS_RUNNING;
struct worker_ctrl  *wctls; 
int WORKER_ISSTATUS(int state) 
{ 
       int i = 0;  
       for (; i < conf_para.MaxClient; i++) 
       { 
       	if(wctls[i].opts.flags == state) 
       	{ 
       		return i; 
       	} 
       }  
       	return -1; 
}

int do_listen()
{
             int s_s, reuse = 1;
	struct sockaddr_in local;
// 建立套接字
	s_s = socket(AF_INET, SOCK_STREAM, 0);
	if (s_s == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
// 初始化
             bzero(&local, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_port = htons(PORT);

             if (setsockopt(s_s, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
      	{
                perror("setsockopet error\n");
                return -1;
     	 }
	int err = bind(s_s, (struct sockaddr*)&local, sizeof(local));
	if (err == -1)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	err = listen(s_s, BACK_LOG);
	if (err == -1)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	return s_s;
}

 static void process_conn_server(int i)
{
	int size = 0, s ;
	char buff[BUFFLEN];  
	s = wctls[i].conn.cs;
              wctls[i].opts.flags = WORKER_DETACHING;
             memset(buff, 0 ,BUFFLEN);
          	 size = read(s, buff, BUFFLEN);
	if(size != 0) 
	{
		printf("buff %s\n", buff);
              }  
              wctls[i].opts.flags = WORKER_IDEL;
	 wctls[i].conn.cs = -1;
	 printf("a clinet [%d] disconnect \n", s);
	 close(s); 
}
static void *handle_request(void *arg)
{
	int  i ;
            i = *(int *)arg;
            wctls[i].opts.flags = WORKER_IDEL;
            	 wctls[i].opts.th = pthread_self(); 
            	 for (;;)
            	 {
            	 	usleep(1000);
            	 	if(pthread_mutex_trylock(&wctls[i].opts.mutex) == 0)
            	 	{
                                process_conn_server(i);
            	 	}
            	 }
}
void worker_add(int i){ 
	printf("add worker %d \n", i);
	pthread_t pt;
	int ret = -1 ;
	ret = pthread_create(&pt,  NULL, (void *)handle_request, (void *)&i);
            if(ret != 0)
            	{ 
            		perror("pthread fail");
            	}

}
void worker_destory(){
	int i ;
	for (i = 0 ; i < conf_para.MaxClient ; i++)
		if(wctls[i].opts.flags != WORKER_DETACHED)
		{
    			pthread_join(wctls[i].opts.th, NULL);
		}
}
static void Worker_Init()
{ 
	printf("==> worker init\n");
	int i = 0;
	 wctls = (struct worker_ctrl *)malloc( sizeof(struct worker_ctrl)*conf_para.MaxClient);
	memset(wctls, 0 , sizeof(*wctls)*conf_para.MaxClient); 
	for (i  = 0; i < conf_para.MaxClient; i++)
	{
		wctls[i].opts.work = &wctls[i];
		wctls[i].conn.work = &wctls[i]; 
		wctls[i].opts.flags = WORKER_DETACHED;  
		pthread_mutex_init(&wctls[i].opts.mutex, NULL);
		pthread_mutex_lock(&wctls[i].opts.mutex);

		wctls[i].conn.con_req.conn = &wctls[i].conn;
		wctls[i].conn.con_res.conn = &wctls[i].conn;
		wctls[i].conn.cs =  -1 ;

		wctls[i].conn.con_req.req.ptr = wctls[i].conn.dreq;
		wctls[i].conn.con_req.head = wctls[i].conn.dreq;
		wctls[i].conn.con_req.uri = wctls[i].conn.dreq;

		wctls[i].conn.con_res.fd = -1 ;
		wctls[i].conn.con_res.res.ptr = wctls[i].conn.dres;
	}
	for (i = 0; i < conf_para.InitClient;i++)
	{
		worker_add(i);
	}
	printf("<== worker init \n");
}
void Worker_ScheduleRun(int s)
{
	printf("==> schedulerun	\n");
	struct sockaddr_in from;
	int len = sizeof(from);
             int i = 0;
	Worker_Init(); 
	for(; STATUS == STATUS_RUNNING; )
	{
		struct timeval tv;
		fd_set rfds;
		int retval = -1;

		FD_ZERO(&rfds);
		FD_SET(s, &rfds);
		tv.tv_sec = 0;
		tv.tv_usec = 500000;

		retval = select(s+1, &rfds, NULL, NULL, &tv);
		switch(retval)
		{
			case -1:
			case 0:
				continue;
				break;
			default:
				if(FD_ISSET(s, &rfds))
				{
					int s_c = accept(s, (struct sockaddr*)&from, &len);  
					char *p = inet_ntoa(from.sin_addr); // why ?
					i = WORKER_ISSTATUS(WORKER_IDEL);  
					if (i == -1)
					{
						i = WORKER_ISSTATUS(WORKER_DETACHED);  
						if (i != -1 )
						{
							worker_add(i); 
						}
						else
						{
							 printf("server busy %d \n", i);
						}
					}
					if(i != -1)
					{
						wctls[i].conn.cs = s_c; 
					             printf("a clent [%d] connect  from [%s], the processer [%d] handle it  \n", s_c, p, i);
						pthread_mutex_unlock(&wctls[i].opts.mutex);
					}
				}
		}
	}
	printf("<==worker schedule run \n"); 
}

int Worker_ScheduleStop()
{
	printf("==> worker schedule stop \n");
	STATUS = STATUS_STOP;
	int  i = 0;
	worker_destory();
	int allfired = 0;
	for (;!allfired;)
	{
		allfired = 1;
		for (i = 0; i < conf_para.MaxClient; i++)
		{
			int flags = wctls[i].opts.flags;
			if (flags == WORKER_DETACHING || flags == WORKER_IDEL)
				allfired = 0;
		}
	} 
	for (i = 0; i < conf_para.MaxClient; i++)
	{
		pthread_mutex_destroy(&wctls[i].opts.mutex);
	}
	free(wctls);

	printf("<== worker schedule stop\n");
	return 0;
}



















