#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
int main (int argc,char* argv[])
{
    int fileOrder = 1;
    while(fileOrder < argc)
    {
        int fd;
        if (( fd = open(argv[fileOrder], O_RDONLY)) < 0)
        {
            printf("cannot open %s \n", argv[fileOrder]);
            exit(1);
        }
        
        char readch;
        int isValid = 1;
        int tmpnum = 0;
        while (read(fd, &readch, 1) > 0)
        {
            if(readch >= '0' && readch <= '9' && isValid)
            {
                tmpnum = tmpnum*10 + (readch -'0');
            }
            else if(readch == '-' || readch == '\r' || readch == '\t' || readch == '\n' || readch == '.' || readch == '/' || readch == ',' )
            {
                if(tmpnum > 0 && (tmpnum % 5 == 0 || tmpnum % 6 == 0))
                {
                    printf("%d\n", tmpnum);
                }
                tmpnum = 0;
                isValid = 1;
            }
            else
            {
                tmpnum = 0;
                isValid = 0;
            }
        }
        if(tmpnum > 0 && (tmpnum % 5 == 0 || tmpnum % 6 == 0) && isValid)
        {
            printf("%d\n", tmpnum);
        }
        fileOrder++;
        close(fd);
    }
    exit(0);
}

/*
int sep(char input)
{
    if(strchr(&input,'-') != 0 || strchr(&input,'\n') != 0 || strchr(&input,'\r') != 0 || strchr(&input,'\t') != 0 || strchr(&input,'.') != 0 || strchr(&input,'/') != 0 || strchr(&input,',') != 0)
    {
	return 1;
    }
    else
    {
        return 0;
    }
}
int main (int argc,char* argv[])
{

    while (i < argc)
    {

        char readch;

        int flag = 1;
        int n;
        while ((n = read(fd, &readch, 1)) > 0)
        {
            if (readch >= '0' && readch <= '9' && flag == 1)
            {
                tmpnum *= 10;
                tmpnum += atoi(&readch);
            }
            else if (sep(readch) == 1)
            {
                if (flag == 1 && tmpnum != 0)
                {
                    if (tmpnum % 5 == 0 || tmpnum % 6 == 0)
                    {
                        printf("%d\n", tmpnum);
                    }
                    tmpnum = 0;
                }
                flag = 1;
            }
            else
            {
                tmpnum = 0;
                flag = 0;
            }
        }
        if (tmpnum != 0 && (tmpnum % 5 == 0 || tmpnum % 6 == 0))
        {
            printf("%d\n", tmpnum);
        }
        i++;
    }
   exit(0);
}
*/