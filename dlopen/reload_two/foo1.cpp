#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

int func1(char *p)
{
   if(p)
   {
	   printf("func1 : [%s]\n",p);
   }
   return 0;
}


#ifdef __cplusplus
}
#endif

 


