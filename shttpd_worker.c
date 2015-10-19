#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#define BUFFLEN 1024
#define PORT 8080
#define BACK_LOG 10
int do_listen()
{
        int s_s;
	struct sockaddr_in local;
// 建立套接字
	s_s = socket(AF_INET, SOCK_STREAM, 0);
	if (s_s == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
// 初始化
        memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_port = htons(PORT);

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
	return err;
}
void Worker_ScheduleRun(int s)
{	
	pid_t pid;
	struct sockaddr_in from;
	for(;;)
	{
		int addrlen = sizeof(struct sockaddr);
		int s_c = accept(s, (struct sockaddr*)&from, &addrlen);
		if(s_c  == -1)
		{
			continue;
		}
		pid = fork();
		if(pid == 0)
		{
			process_conn_server(s_c);	
		}	
		else
		{
			close(s_c);
		}
	}
}
void process_conn_server()
{
	ssize_t size = 0;
	char buff[BUFFLEN];

}
