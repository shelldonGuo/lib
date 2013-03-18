#ifndef _REDIS_CONN_H_
#define _REDIS_CONN_H_

#include "hiredis.h"
#include <ul_log.h>
#include <ul_func.h>
#include <ul_conf.h>

#define MAX_STRLEN	512
#define DEFAULT_CONF_NUM 30
#define DEFAULT_CONFPATH_REDIS "./conf/"
#define DEFAULT_CONFFILE_REDIS "redis.conf"
#define DEFAULT_REDIS_SERVER_IP "localhost"
#define DEFAULT_REDIS_PORT 6379
#define MAX_REDIS_CONTENT_LEN 102400

typedef unsigned long long uint64;
typedef unsigned int uint32;


#pragma pack(1)
typedef enum
{
	OK	= 0,
	FAIL	= -1,
	CONNECT_ERROR	= -2 ,
}ENUM_REDIS_ERROR;

struct RedisConf
{
	RedisConf()
	{
		port=0;
        ip[0] = '\0';
	}
	
	int port;
	char ip[MAX_STRLEN];
};
#pragma pack()

typedef struct _Redis_Conf_t
{
	char confpath[255];
	char conffile[255];
	struct RedisConf redis_conf;
}Redis_Conf_t;

class RedisResult
{
protected:
   redisReply* _result;
	
public:
    RedisResult()
    {
        _result = NULL;
    }

    RedisResult(redisReply* resultSet)
    {
        _result = resultSet;
    }
    ~RedisResult()
    {
        if (_result != NULL)
		{
			freeReplyObject(_result);
			_result = NULL;
        }

    }
    void set(redisReply* rst) 
    {
        if (_result != NULL)
        {
            freeReplyObject(_result);
            _result = NULL;
        }

        _result = rst;
    }
    redisReply* get() { return _result; }
};

// the redis connection class
class RedisConn
{
public:
	RedisConn();
	~RedisConn();

	int initConn(RedisConf &redis_conf);
	void finiConn();

	redisContext* getRedis();

	int execCmd(const char* stmt, RedisResult *result);
private:
	int reconnect();
	RedisConf *_pconf;    
	redisContext* _conn;

};


extern RedisConn redis_conn;
extern Redis_Conf_t redis_conf;
int redis_load_conf();
int redis_conns_init();

#endif 
