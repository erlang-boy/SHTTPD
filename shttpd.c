
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "shttpd.h"

int params_Init(int argc, char ** argv);

int do_listen();
void Worker_ScheduleRun(int s);
int Worker_ScheduleStop();
// static void sig_int( int num )
// {
// 	Worker_ScheduleStop();
// 	return;
// }
int main(int argc, char * argv[])
{
    // signal(SIGINT, sig_int);
   params_Init(argc, argv);
   int s = do_listen();
   Worker_ScheduleRun(s);

    return 0;
}

