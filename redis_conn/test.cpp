#include <stdio.h>
#include "redis_conn.h"
int main()
{
	RedisResult res;

	int  ret;
	if ( ret=redis_conns_init() )
	{
		printf("init faild[%d]\n",ret);
		exit (-1);
	}
	if(ret=redis_conn.execCmd("setex key 10 val",NULL) )
	{
		printf("exec faild[%d]\n",ret);
		exit (-1);
	}
	if (ret = redis_conn.execCmd("get key",&res) )
	{
		printf("exec faild[%d]\n",ret);
		exit (-1);
	}
	printf("%s\n",res.get()->str);
	return 0;
}
