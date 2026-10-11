#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int N;
    if (argc != 2){
        printf("using: countdown N\n");
        exit(1);
    }

    N = atoi(argv[1]);

    while (N > 0){
        printf("%d\n", N);
        pause(20);
        N--;
    }

    pause(20);
    printf("Поехали!\n");
    exit(0);
}
