#include "kernel/types.h"
#include "user/user.h"

int
main()
{
    int my_pid = getpid();
    printf("my pid: %d\n", my_pid);
    printf("children before fork: %d\n", count_children());
    for (int i = 0; i < 3; i++){
        int pid = fork();
        if (pid == 0){
            pause(10);
            printf("[child] pid=%d ppid=%d\n", getpid(), getppid());
            pause(150);
            exit(0);
        }
    }
    printf("children after fork: %d\n", count_children());

    for (int i = 0; i < 3; i++){
        wait(0);
    }

    printf("children after wait: %d\n", count_children());

    return(0);
}
