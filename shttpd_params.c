#include <stdio.h>
#include "shttpd.h"
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>

struct conf_opts conf_para = {
     "/usr/local/var/www/cgi-bin",  //cgi根路径
     "index.html",         // 默认文件名
     "/usr/local/var/www", //文件的根路径
     "/etc/SHTTPD.conf", //配置文件
     8080,  //端口
     4,   //最大连接数
     3, //超时时间
     2 //初始化线程池线程数
};
static char *shortopts = "c:d:f:ho:l:m:t:";

static struct option longopts[] = {
    {"CGIRoot",required_argument, NULL,'c'},
    {"ConfigFile",        required_argument, NULL, 'f'},
    {"DefaultFile" ,       required_argument, NULL, 'd'},
    {"DocumentRoot", required_argument, NULL, 'o'},
    {"ListenPort",     required_argument, NULL, 'l'},
    {"MaxClient",    required_argument, NULL, 'm'},
    {"TimeOut,",    required_argument, NULL, 't'},
    {"Help", no_argument, NULL , 'h'},
    {0,          0,                      0,           0}
};

static char * l_opt_arg;
void display_usage()
{
    printf("SHTTPD --ListenPort number --MaxClient number --DocumentRoot path --CGIRoot \
path --DefaultFile filename --Timeout seconds --ConfigFile filename");
};
/***********************************************
 *
 *SHTTPD --ListenPort number --MaxClient number --DocumentRoot path --CGIRoot
 *path --DefaultFile filename --Timeout seconds --ConfigFile filename
 *
 ***********************************************/
static int Param_CmdParse(int argc,  char ** argv)
{
        int c;
        int len = 0;
        int value = 0 ;

        while((c = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1)
        {
                switch (c)
                {
                 case 'c':
                    l_opt_arg =  optarg;
                    if (l_opt_arg && l_opt_arg[0] != ':')
                       {
			       len = strlen(l_opt_arg);
			       memcpy(conf_para.CGIRoot, l_opt_arg, len + 1);
			       printf( "c================> %s==%d--%d\n", optarg, value, len);
		       }
                    break;
		 case 'd':
                    l_opt_arg =  optarg;
                    if (l_opt_arg && l_opt_arg[0] != ':')
                       {
			       len = strlen(l_opt_arg);
			       memcpy(conf_para.DefaultFile, l_opt_arg, len + 1);
			       printf( "c================> %s==%d--%d\n", optarg, value, len);
		       }
                    break;
		 case 'f':
                    l_opt_arg =  optarg;
                    if (l_opt_arg && l_opt_arg[0] != ':')
                       {
			       len = strlen(l_opt_arg);
			       memcpy(conf_para.ConfigFile, l_opt_arg, len + 1);
			       printf( "c================> %s==%d--%d\n", optarg, value, len);
		       }
                    break;
		 case 'o':
                    l_opt_arg =  optarg;
                    if (l_opt_arg && l_opt_arg[0] != ':')
                       {
			       len = strlen(l_opt_arg);
			       memcpy(conf_para.DocRoot, l_opt_arg, len + 1);
			       printf( "c================> %s==%d--%d\n", optarg, value, len);
		       }
                    break;
                 case 'l':
                    l_opt_arg =  optarg;
                    if (l_opt_arg && l_opt_arg[0] != ':')
                       {
			       len = strlen(l_opt_arg);
			       value = strtol(l_opt_arg, NULL, 10);
			       if (value != LONG_MAX && value != LONG_MIN)
				       conf_para.ListenPort = value;
			       printf( "c================> %s==%d--%d\n", optarg, value, len);
		       }
                    break;
                 case 'm':
                    l_opt_arg =  optarg;
                    if (l_opt_arg && l_opt_arg[0] != ':')
                       {
			       len = strlen(l_opt_arg);
			       value = strtol(l_opt_arg, NULL, 10);
			       if (value != LONG_MAX && value != LONG_MIN)
				       conf_para.MaxClient = value;
			       printf( "c================> %s==%d--%d\n", optarg, value, len);
		       }
                    break;
	         case 't':
                    l_opt_arg =  optarg;
                    if (l_opt_arg && l_opt_arg[0] != ':')
                       {
			       len = strlen(l_opt_arg);
			       value = strtol(l_opt_arg, NULL, 10);
			       if (value != LONG_MAX && value != LONG_MIN)
				       conf_para.Timeout = value;
			       printf( "c================> %s==%d--%d\n", optarg, value, len);
		       }
                    break;
	          case '?':
			       printf( "c================> %s==%d--%d\n", optarg, value, len);
	          case 'h':
			       display_usage();
			       break;

                }
        }
        return 0;
};

void Para_FileParse(char *file)
{
     char  * line = NULL;
     size_t len =  0;
     size_t n = 0;
     char *name = NULL, *value = NULL;
     FILE *fd = NULL;
      fd = fopen(file, "r");
     if( NULL == fd  )
     {
	     exit(EXIT_FAILURE);
     }
     while ((n = getline(&line, &len, fd))  !=  -1 )
     {
            char * pos = line ;
            printf("line:%s\n", line);
            while (isspace(*pos))
            {
                pos++;
            }
            if (*pos == '#')
            {
                continue;
            }
            name = pos ;
            while ( *pos != '=' && !isspace(*pos))
            {
                    pos++ ;
            }
                     *pos = '\0';
            printf("name:%s\n", name);
                     pos ++;
            while (isspace(*pos) || (*pos == '='))
            {
                pos++;
            }
            value = pos;
            while (!isspace(*pos) && *pos != '\r' && *pos != '\n')
            {
                pos++ ;
            }
            *pos = '\0';
            printf("value:%s\n", value);
            if (strncmp("CGIRoot", name , 7) == 0)
            {
                memcpy(conf_para.CGIRoot, value, strlen(value) + 1);
            }
            else if(strncmp("DefaultFile", name , 11)== 0)
            {
                memcpy(conf_para.DefaultFile, value , strlen(value) + 1);
            }

            else if(strncmp("DocRoot", name , 7)== 0)
            {
                memcpy(conf_para.DocRoot, value, strlen(value) + 1 );
            }

            else if(strncmp("ListenPort", name, 10)== 0)
            {
                conf_para.ListenPort = strtol(value, NULL, 10);
            }

            else if(strncmp("MaxClient", name, 9)== 0)
            {
                    conf_para.MaxClient = strtol(value, NULL, 10);
            }

           else if(strncpy("TimeOut", name , 7)== 0)
            {
                conf_para.Timeout = strtol(value, NULL, 10);
            }
            else
            {
               printf("else:%d\n", strncmp("CGIRoot", name , 7) );
            }
     }
    close(fd);
    fd = NULL;
}


int params_Init(int argc, char ** argv)
{

   Param_CmdParse(argc, argv);
   printf( "params_Init :Param_CmdParse======> %s\n",conf_para.ConfigFile );
   Para_FileParse( conf_para.ConfigFile);
   printf( "params_Init :Para_FileParse======> %s\n",conf_para.CGIRoot );
   return 0;
}










