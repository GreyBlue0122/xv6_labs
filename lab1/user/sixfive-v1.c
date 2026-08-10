#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

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
    if (argc < 2)
	exit(1);
    int i = 1;
    while (i < argc)
    {
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0)
        {
            printf("cannot open this file\n");
            exit(1);
        }

        char readch;
        int tmpnum = 0;
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
