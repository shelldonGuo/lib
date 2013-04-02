#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

int isreload = 0;

void sig_show(int s)
{
	printf("catched signal: %d\n", s);
	return;
}

void sig_reloadso(int s)
{
	printf("catched signal: %d\n", s);
	isreload = 1;
	printf("sigfunc isreload ? %d\n", isreload);
	return;
}

int main(int argc, char *argv [])
{
	struct sigaction show;
	show.sa_handler = &sig_show;
	show.sa_flags = SA_NOMASK;
	show.sa_restorer = 0;
	if(sigaction(3, &show, 0) == -1)
	{
		printf("sigaction failed. errno: %d\n", errno);
		return 0;
	}

	struct sigaction reload;
	reload.sa_handler = &sig_reloadso;
	reload.sa_flags = SA_NOMASK;
	reload.sa_restorer = 0;
	if(sigaction(4, &reload, 0) == -1)
	{
		printf("sigaction failed. errno: %d\n", errno);
		return 0;
	}


	void *libfoo2;
	int (*foo2)(char *x);

	const static char * h = "hello";
	char buf[200];

	if((libfoo2 = dlopen("./libfoo2.so", RTLD_NOW | RTLD_GLOBAL)) != 0)
	{
		foo2 = (int (*)(char *)) dlsym(libfoo2, "func2");
		//foo2 =  dlsym(libfoo2, "func2");
		if(dlerror())
		{
			printf("error? %s\n", dlerror());
		}
	}
	else
	{
		printf("can not open libfoo2.so\n");
		return 0;
	}

	foo2("test test");

	int i=0;

	while(1)
	{
		printf("isreload ? %d\n", isreload);

		if(isreload)	//test if need reload
		{
			dlclose(libfoo2);

			if((libfoo2 = dlopen("./libfoo1.so", RTLD_LAZY | RTLD_GLOBAL)) != 0)
			{
				foo2 = (int (*)(char *)) dlsym(libfoo2, "func1");
				//foo2 = dlsym(libfoo2, "func2");
				if(dlerror())
				{
					printf("error? %s\n", dlerror());
					return 0;
				}
			}

			isreload = 0;
			printf("successfully reload libfoo2.so\n");
		}

		++i;

		sprintf(buf, "%s %d", h, i);	
		buf[199]='\0';

		foo2(buf);		//from foo*

		sleep(4);
	}
	
	return 0;
}


