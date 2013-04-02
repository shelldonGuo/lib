#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

int func2(char *p)
{
   if(p)
   {
	   printf("func2 : [%s]\n",p);
   }
   return 0;
}

/*
int main(){
	func2("hello");
	return 0;
}
*/

 
#ifdef __cplusplus
}
#endif

 


