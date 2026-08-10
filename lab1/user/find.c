#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
void find (char *path,char *fname,char** argv)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    if((fd = open(path, O_RDONLY)) < 0)
    {
        fprintf(2, "cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0)
    {
        fprintf(2, "cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type)
    {
    case T_FILE:
        char *name = path +strlen(path);
        while(*name != '/')
        {
            name--;
        }
        name++;
        if(strcmp(fname,name) == 0)
        {
            if(!argv)
            {
                printf("%s\n", path);
            }
            else
            {
                int pid = fork();
                if(pid > 0)
                {
                    wait(0);
                }
                else if(pid == 0)
                {
                    char *newargv[10];
                    int i = 0;
                    while((argv[i]) && (i < 8))
                    {
                        newargv[i] = argv[i];
                        i++;
                    }
                    newargv[i++] = path;
                    newargv[i] = 0;

                    exec(argv[0],newargv);
                }
            }
        }
            
        break;

    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if(de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0; 
            if(strcmp(de.name,".") == 0 || strcmp(de.name,"..") == 0)
            {
                continue;
            }
            find(buf,fname,argv);
        }
        break;
    }
    close(fd);
}
int main(int argc,char*argv[])
{
    if(argc < 3)
    {
        printf("Parameter leck\n");
        exit(1);
    }
    char *dirname = argv[1];
    char *filename = argv[2];

    if (strcmp(argv[3],"-exec") == 0)
    {
        char** myargv = argv + 4; 
        find(dirname,filename,myargv);
    }
    else
    {
        find(dirname,filename,0);
    }
    exit(0);

}