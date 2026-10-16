#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    printf("Number children alive: %d\n", count_children());
    return 0;
}
