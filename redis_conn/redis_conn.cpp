#include <string.h>
#include <stdlib.h>

#include "redis_conn.h"

RedisConn redis_conn;
Redis_Conf_t redis_conf;

RedisConn::RedisConn() : _conn(NULL)
{
}

RedisConn::~RedisConn()
{
}

int RedisConn::initConn(RedisConf &redis_conf)
{
	_pconf = &redis_conf;
	if ( NULL == _pconf  || '\0' == _pconf->ip[0] || 0 == _pconf->port)
		return FAIL;
	_conn = redisConnectWithTimeout(_pconf->ip, _pconf->port, _pconf->conn_timeout); //redis server默认端口
	if(_conn->err){
		printf("connection error: %s\n", _conn->errstr);
		return CONNECT_ERROR;
	}

	if(redisSetTimeout(_conn, _pconf->rdwr_timeout))
	{
		printf("redis set time error\n");
		return CONNECT_ERROR;
	}

    return OK;
}

void RedisConn::finiConn()
{
	redisFree(_conn);
	_conn = NULL;
}

redisContext* RedisConn::getRedis()
{
    return _conn;
}

int RedisConn::execCmd(const char* stmt, RedisResult *result)
{
	redisContext* conn = getRedis();
	if (NULL == conn)
	{
		return FAIL;
	}

	redisReply *reply = (redisReply *) redisCommand(conn, stmt);
	if(NULL == reply)
	{
		if(reconnect() == OK)
		{
			conn = getRedis();
			reply = (redisReply *) redisCommand(conn, stmt);
			if (NULL == reply)
			{
				return FAIL;
			}
		}		
		else
		{
			return CONNECT_ERROR;
		}
	}

	if (NULL != result )
		result->set(reply);

	return OK;
}

int RedisConn::reconnect()
{
	finiConn();

	if ( NULL == _pconf  || '\0' == _pconf->ip[0] || 0 == _pconf->port)
		return FAIL;

	_conn = redisConnectWithTimeout(_pconf->ip, _pconf->port, _pconf->conn_timeout); //redis server默认端口
	if(_conn->err){
		printf("connection error: %s\n", _conn->errstr);
		return CONNECT_ERROR;
	}

	if(redisSetTimeout(_conn, _pconf->rdwr_timeout))
	{
		printf("redis set time error\n");
		return CONNECT_ERROR;
	}
	
    return OK;

}

int redis_load_conf()
{
	Ul_confdata *pconf;
	pconf=ul_initconf(DEFAULT_CONF_NUM);
	if(!pconf)
	{
		printf("\n[Fatal Error] init config file error! \n");
		return FAIL;
	}
	memset(&redis_conf,0,sizeof(redis_conf));

	strcpy(redis_conf.confpath,DEFAULT_CONFPATH_REDIS);
	strcpy(redis_conf.conffile,DEFAULT_CONFFILE_REDIS);

	int ret=ul_readconf(redis_conf.confpath,redis_conf.conffile,pconf);
	if(ret<0){
		printf("\n[Fatal Error] load config file error! path[%s] file[%s] \n",redis_conf.confpath,redis_conf.conffile);
		return FAIL;
	}

	if (ul_getconfstr(pconf, "ip", redis_conf.redis_conf.ip)) 
	{
		printf("Conf: redis server ip: %s\n",redis_conf.redis_conf.ip);
		ul_writelog(UL_LOG_NOTICE, "Conf: redis server ip: %s", redis_conf.redis_conf.ip);
	}
	else 
	{
		strcpy( redis_conf.redis_conf.ip,DEFAULT_REDIS_SERVER_IP);
		ul_writelog(UL_LOG_NOTICE, "Conf: redis server ip is: %s DEFAULT", DEFAULT_REDIS_SERVER_IP);
	}

	if (ul_getconfint(pconf, "port",&( redis_conf.redis_conf.port))) 
	{
		ul_writelog(UL_LOG_NOTICE, "Conf: redis port: %d", redis_conf.redis_conf.port);
	}
	else 
	{
		redis_conf.redis_conf.port = DEFAULT_REDIS_PORT;
		ul_writelog(UL_LOG_NOTICE, "Conf: redis port is: %d DEFAULT", DEFAULT_REDIS_PORT);
	}

	if (ul_getconfint(pconf, "conn_timeout",(int *)&( redis_conf.redis_conf.conn_timeout.tv_sec))) 
	{
		ul_writelog(UL_LOG_NOTICE, "Conf: redis conn_timeout: %d", redis_conf.redis_conf.conn_timeout.tv_sec);
	}
	else 
	{
		redis_conf.redis_conf.port = DEFAULT_CONN_TIMEOUT;
		ul_writelog(UL_LOG_NOTICE, "Conf: redis conn_timeout is: %d DEFAULT", DEFAULT_CONN_TIMEOUT);
	}

	if (ul_getconfint(pconf, "rdwr_timeout",(int *)&( redis_conf.redis_conf.rdwr_timeout.tv_sec))) 
	{
		ul_writelog(UL_LOG_NOTICE, "Conf: redis rdwr_timeout: %d", redis_conf.redis_conf.rdwr_timeout.tv_sec);
	}
	else 
	{
		redis_conf.redis_conf.port = DEFAULT_RDWR_TIMEOUT;
		ul_writelog(UL_LOG_NOTICE, "Conf: redis rdwr_timeout is: %d DEFAULT", DEFAULT_RDWR_TIMEOUT);
	}




	ul_freeconf(pconf);
	pconf=NULL;

	return OK;
}

int redis_conns_init()
{
	if(redis_load_conf() != OK)
	{
		printf("\nredis_load_conf failed!\n");
		return FAIL;
	}

	int ret;
	ret = redis_conn.initConn(redis_conf.redis_conf);
	if(OK != ret)
	{
		printf("redis conn init error.ip=[%s], port=[%d]",redis_conf.redis_conf.ip,redis_conf.redis_conf.port);
		return FAIL;
	}

	return OK;
}

