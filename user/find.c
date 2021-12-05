#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* filename(char *dir) {
  char *p;

  // Find first character after last slash.
  for(p=dir+strlen(dir); p >= dir && *p != '/'; p--)
    ;
  p++;

  return p;
}

void find(char* dir, char* target){
    int fd;
    char *name, buf[512];
    struct stat st;
    struct dirent de;
    if((fd = open(dir, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", dir);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", dir);
        close(fd);
        return;
    }
    switch (st.type)
    {
    case T_FILE:
        name = filename(dir);
        if ( strcmp(name, target) == 0 ){
            printf("%s\n", dir);
        }
        break;
    case T_DIR:
        strcpy(buf, dir);
        name = buf + strlen(buf);
        *name++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0 || strcmp(de.name,".") == 0 || strcmp(de.name,"..") == 0) {
                continue;
            }
            memmove(name, de.name, DIRSIZ);
            name[DIRSIZ] = 0;
            find(buf, target);
        }
        break;
    }
    close(fd);

}

int
main(int argc, char *argv[]){
    if (argc < 3){
        printf("illegal parameters.\n");
        exit(1);
        return 0;
    }
    
    for (int i = 2; i < argc; i++){
        find(argv[1], argv[i]);
    }
    exit(0);
}

