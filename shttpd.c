
#include <stdio.h>
#include <stdlib.h>
#include "shttpd.h"

int params_Init(int argc, char ** argv);

int main(int argc, char * argv[])
{
   params_Init(argc, argv);
   int s = do_listen();
   Worker_ScheduleRun(s);

    return 0;
}

