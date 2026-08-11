#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"


int
main(int argc, char *argv[])
{
  // Your code here.
  char* p = sbrk(4096);
  int flag = 0;
  char buffer[15];
  char* bp = buffer;
  buffer[14] = '\0';
  int i = 0;
  uint8* q = (uint8*)p;
  while(i++<4096)
  {

    if(*p >= '\x20' && *p <= '\x7e')
    {
      if(bp - buffer < 14)
      {
        *bp = *p;
        bp++; 
        if(bp - buffer == 14)
        {
          if(strcmp(buffer,"This may help.") != 0)
          {
            printf("%s\n",buffer);
            bp = buffer;
          }
        }
      }
      else
      {
        printf("%c",*p);
        if(!flag)
        {
          flag = 1;
        }
      }
    }
    else if(flag)
    {
      printf("\n");
      break;
    }
    p++;
    q++;
  }

  exit(1);
}
