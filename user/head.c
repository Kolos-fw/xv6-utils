#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
    if (argc != 3){
        printf("using: head N file\n");
        exit(1);
    }

    int fd = open(argv[2], O_RDONLY);
    if (fd == -1){
        printf("Error: cannot open\n");
        exit(1);
    }

    int N = atoi(argv[1]);
    char buffer[512];
    int n_read;
    int lines = 0;
    int flag = 0;
    while ((n_read = read(fd, buffer, 512)) > 0 && flag != 1){
        for (int i = 0; i < n_read; i++){
            write(1, &buffer[i], 1);
            if (buffer[i] == '\n'){
                lines++;
                if (lines == N){
                    flag = 1;
                    break;
                }
            }
        }    
    }
    close(fd);
}
