#include <string.h>
#include <stdlib.h>

#include "redis_conn.h"

RedisConn redis_conn;
Redis_Conf_t redis_conf;

const int RedisConn::m_retry = 2;

RedisConn::RedisConn() : m_conn(NULL)
{
}

RedisConn::~RedisConn()
{
}

int RedisConn::loadConf(RedisConf &redis_conf)
{
	m_pconf = &redis_conf;
	if ( NULL == m_pconf  || '\0' == m_pconf->ip[0] || 0 == m_pconf->port)
		return FAIL;
	return OK;
}

int RedisConn::initConn(RedisConf &redis_conf)
{
	if(loadConf(redis_conf) != OK)
		return FAIL;
	if(reconnect() != OK)
		return FAIL;

	return OK;
}

void RedisConn::finiConn()
{
	if (m_conn != NULL)
	{
		redisFree(m_conn);
		m_conn = NULL;
	}
}

redisContext* RedisConn::getRedis()
{
    return m_conn;
}

int RedisConn::execCmd(const char* stmt, RedisResult *result)
{
	int retry = m_retry;
	redisReply *reply;

	while(retry > 0)
	{
		if (NULL == m_conn)
		{
			if(reconnect() != OK)
			{
				retry--;
				continue;
			}
		}

		reply = (redisReply *) redisCommand(m_conn, stmt);
		if(NULL == reply)
		{
			finiConn();
			continue;
		}
		break;
	}

	if (retry <= 0)
	{
		return FAIL;
	}

	if (NULL != result )
		result->set(reply);

	return OK;
}

int RedisConn::reconnect()
{
	if ( NULL == m_pconf  || '\0' == m_pconf->ip[0] || 0 == m_pconf->port)
		return FAIL;

	finiConn();

	m_conn = redisConnectWithTimeout(m_pconf->ip, m_pconf->port, m_pconf->conn_timeout); //redis serverÄ¬ÈÏ¶Ë¿Ú
	if ( NULL == m_conn )
	{
		printf("connection init error: conn==NULL\n");
		return CONNECT_ERROR;
	}
	if(m_conn->err){
		printf("connection error: %s\n", m_conn->errstr);
		finiConn();
		return CONNECT_ERROR;
	}

	if(redisSetTimeout(m_conn, m_pconf->rdwr_timeout))
	{
		printf("redis set time error\n");
		finiConn();
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
		redis_conf.redis_conf.conn_timeout.tv_sec = DEFAULT_CONN_TIMEOUT;
		ul_writelog(UL_LOG_NOTICE, "Conf: redis conn_timeout is: %d DEFAULT", DEFAULT_CONN_TIMEOUT);
	}

	if (ul_getconfint(pconf, "rdwr_timeout",(int *)&( redis_conf.redis_conf.rdwr_timeout.tv_sec))) 
	{
		ul_writelog(UL_LOG_NOTICE, "Conf: redis rdwr_timeout: %d", redis_conf.redis_conf.rdwr_timeout.tv_sec);
	}
	else 
	{
		redis_conf.redis_conf.rdwr_timeout.tv_sec = DEFAULT_RDWR_TIMEOUT;
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

