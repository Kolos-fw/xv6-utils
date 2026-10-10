#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

void find(char *path, char *name)
{
    struct stat st;
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        printf("Error: cannot open file\n");
        return;    
    }
    int type = fstat(fd, &st);
    if (type < 0) {
        printf("Error type\n");
        return;    
    }

    if (st.type == T_FILE){
        char *short_name;
        short_name = path;
        for (int i = strlen(path) - 1; i >= 0; i--){
            if (path[i] == '/'){
                short_name = &path[i + 1];
            }
        }
        printf("DEBUG: comparing [%s] with [%s]\n", short_name, name);
        int compare = strcmp(short_name, name);
            if (compare == 0){
                printf("%s\n", path);
            }
        close(fd);
    }

    if (st.type == T_DIR){
        printf("DEBUG: entering directory %s\n", path);
        close(fd);
    }
}

int 
main(int argc, char *argv[]) 
{
    if (argc != 3) {
        printf("usage: find <dir> <name>\n");
        exit(1);
    }
    printf("DEBUG: path=%s, name=%s\n", argv[1], argv[2]);
    find(argv[1], argv[2]);
    exit(0);
}
