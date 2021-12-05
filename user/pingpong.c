#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]){
    // p1: child send to father
    // p2: father send to child
    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);
    if (fork() == 0) {
        // child
        char *buf = (char *)(malloc(1));
        int child_pid;
        //  child read p2
        read(p2[0], buf, 1);
        child_pid = getpid();
        printf("%d: received ping\n", child_pid);
        //  child write p1
        write(p1[1], " ", 1);
    }else {
        // father
        char *buf = (char *)(malloc(1));
        int father_pid, status;
        // father send p2 
        write(p2[1], " ", 1);
        // father read p1
        read(p1[0], buf, 1);
        father_pid = getpid();
        printf("%d: received pong\n", father_pid);
        wait(&status);
    }
    exit(0);

}