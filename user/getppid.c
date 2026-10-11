#include "kernel/types.h"
#include "user/user.h"

int
main()
{
    int ppid = getppid();
    printf("My parent PID is: %d", ppid);
    exit(0);
}
