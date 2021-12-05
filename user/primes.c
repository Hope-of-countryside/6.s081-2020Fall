#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void create_child_process();
int main() {
    int p[2];
    pipe(p);
    if (fork() != 0) {
        close(p[0]); // close read
        for (int i = 2; i <= 35; i++) {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]); // close write
        wait(0);
    } else {
       create_child_process(p);
    }
    exit(0);
}

void create_child_process(int p[]) {
    int x, y;
    int child_p[2];
    
    close(p[1]);               // close write
    if (read(p[0], &x, sizeof(int))) {
        fprintf(1, "prime %d\n", x);
        pipe(child_p);
        if (fork() != 0) {
            close(child_p[0]); // close child read
            while (read(p[0], &y, sizeof(int))) {
                if (y % x != 0) {
                    write(child_p[1], &y, sizeof(int));
                }
            }
            close(p[0]);       // close read
            close(child_p[1]); // close child write
            wait(0);
        } else {
            create_child_process(child_p);
        }
    }
    exit(0);
}