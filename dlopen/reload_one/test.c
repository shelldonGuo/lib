/*test.c*/
#include "stdio.h"
#include "stdlib.h"
#include "dlfcn.h"    //��Ҫ��ͷ�ļ�

int main(void)
{
	void *handle;
	int (*fcn)(int x, int y);
	const char *errmsg;

	while (1)
	{
		/* open the library */   //�򿪿�
		handle = dlopen("libsthc.so", RTLD_LAZY|RTLD_LOCAL);
		if(handle == NULL)
		{
			fprintf(stderr, "Failed to load libsthc.so: %s\n", dlerror());
			return 1;
		}

		//*(void **)(&fcn) = dlsym(handle, "add");            //ok
		fcn = dlsym(handle, "add");                                   //ok
		if((errmsg = dlerror()) != NULL)
		{
			printf("%s\n", errmsg);
			return 1;
		}
		printf("%d\n", fcn(9, 5));
		dlclose(handle);
		if (handle!=NULL)
		{
			printf("handle=[%u]\n",handle);
		}
		sleep(5);
	}

	return 0;
}
