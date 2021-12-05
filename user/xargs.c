#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"
int
main(int argc, char *argv[]){
    char buf[1024];
    char *xargs_argv[MAXARG];

    for (int i = 1; i < argc; i++) {
        xargs_argv[i - 1] = argv[i];
    }
    while(1){
        int index = 0;
        while (read(0, &buf[index], sizeof(char)) ) {
            if (buf[index] == '\n') {
                break;
            }
            index++;
            
        }
        
        if (index == 0) {
            break;
        }
        buf[index] = 0;
        // printf("buf: %s\n", buf);
        xargs_argv[argc - 1] = buf;
        if(fork() == 0) {
            exec(xargs_argv[0], xargs_argv);
            exit(0);
        }else{
            wait(0);
        }
    }
    exit(0);
}