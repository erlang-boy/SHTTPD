
#ifndef __SHTTPD__
#define __SHTTPD__

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


struct conn_response {
	
};

struct conn_request {
	
};


struct worker_opts{
    pthread_t th;
    int flags;
    pthread_mutex_t mutex;
    struct worker_ctl *work;
};

struct worker_conn{
    #define K 1024
	char dreq[16*K];
	char dres[16*K];
	int cs;
	int to;
	struct conn_response con_res;
	struct conn_request con_req;
	struct worker_ctl *work;
};

struct worker_ctl{
    struct worker_opts opts;
    struct worker_conn conn;
};
#endif
// __SHTTPD__
