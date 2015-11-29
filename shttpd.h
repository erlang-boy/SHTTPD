
#ifndef __SHTTPD__
#define __SHTTPD__
#include <pthread.h>

typedef enum {
       METHOD_GET ,
       METHOD_POST,
       METHOD_PUT,
       METHOD_HEAD
} SHTTPD_METHOD_TYPE;

enum
{
	WORKER_DETACHED = 0,
	WORKER_IDEL,
	WORKER_DETACHING

} ;
struct conf_opts{
    char CGIRoot[128];
    char DefaultFile[128];
    char DocRoot[128];
    char ConfigFile[128];

    int ListenPort;
    int MaxClient;
    int Timeout;
    int InitClient;
};


typedef struct vec
{
	char  const *ptr;
	int      len;
	SHTTPD_METHOD_TYPE type;
}vec;

struct headers{
	 int cl; //内容长度
	 int ct; //内容类型
	 int connection; //连接状态
	 int ims; //最后修改时间
	 int user; //用户名称
	 int auth; //权限
	 int useragent; //用户
	 int referer; //参考
	 int cookie; // cookie
	 int location; // 位置
	 int range;  //范围
	 int status; //状态值
	 int transenc; //编码类型
};
struct conn_response {
	struct vec res;
	time_t birth_time;
	time_t expire_time;
	int status;
	int cl;
	int fd;
	// struct stat fstate;
	struct worker_conn *conn;

};

struct conn_request {
	struct vec req;
	char *head;
	char *uri;
	// char rpath[URI_MAX];//todo
	int method;
	unsigned long major;
	unsigned long minor;
	struct headers ch;
	struct worker_conn *conn;
	int err;
};


struct worker_opts{
    pthread_t th;
    int flags;
    pthread_mutex_t mutex;
    struct worker_ctrl *work;
};

struct worker_conn{
    #define K 1024
	char  dreq[16*K]; // 存放接受到的客户端请求数据
	char dres[16*K];// 存放发送给客户端的数据
	int cs;// 与客户端连接的套接字
	int to;// 超时响应时间
	struct conn_response con_res;// 维护响应结构
	struct conn_request con_req;// 维护客户端的请求
	struct worker_ctrl *work;// 本线程的控制结构
};

struct worker_ctrl{
    struct worker_opts opts;
    struct worker_conn conn;
};

static struct conf_opts conf_para = {
     "/usr/local/var/www/cgi-bin",  //cgi根路径
     "index.html",         // 默认文件名
     "/usr/local/var/www", //文件的根路径
     "/etc/SHTTPD.conf", //配置文件
     8080,  //端口
     4,   //最大连接数
     3, //超时时间
     1 //初始化线程池线程数
};
static struct vec _shttpd_methods[] = {
	{"GET", 3, METHOD_GET},
	{"POST",4, METHOD_POST},
	{"PUT", 5, METHOD_PUT},
	{"HEAD",4, METHOD_HEAD},
	{NULL,0}
};
#endif
// __SHTTPD__
