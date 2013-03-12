#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "errmsg.h"
#include "mysql_conn.h"
#include "ul_conf.h"


MysqlConn mysql_conn;
Mysql_Conf_t mysql_conf;

MysqlConn::MysqlConn() : conn_(NULL)
{
}

MysqlConn::~MysqlConn()
{
}

int MysqlConn::initConn(DBConf &db_conf)
{
	pconf_ = &db_conf;
	conn_ = mysql_init(NULL);
	if(!conn_)
	{
		return -1;
	}

	if (mysql_real_connect(conn_, 
					strlen(pconf_->ip_)> 0 ? pconf_->ip_: NULL,
					strlen(pconf_->user_) > 0 ? pconf_->user_ : NULL,
					strlen(pconf_->pswd_)> 0 ? pconf_->pswd_: NULL,
					NULL,
					pconf_->port_,
					strlen(pconf_->socket_) > 0 ? pconf_->socket_: NULL,
					0) == NULL)
	{
		return -1;
	}
	
    return 0;
}

void MysqlConn::finiConn()
{
	mysql_close(conn_);
}

MYSQL* MysqlConn::getMysql()
{
    return conn_;
}

void MysqlConn::returnMysql(MYSQL* mysql)
{   
}

int MysqlConn::execUpdate(const char* stmt, int& insertID, int& affected)
{
	MYSQL* conn = getMysql();
	if (NULL == conn)
	{
		return SQL_FAIL;
	}

	int ret;
	ret = mysql_query(conn, stmt);
	if(ret)
	{
		if((ret = reconnect()) ==0)
		{
			conn = getMysql();
			ret = mysql_query(conn, stmt);
			if (ret)
			{
				ret = SQL_FAIL;
			}
		}		
		else
		{
			ret = SQL_CONNECT_ERROR;
		}
	}
	if(ret)
	{
		returnMysql(conn);
		return ret;
	}	

	insertID = mysql_insert_id(conn);
	affected = mysql_affected_rows(conn);

	returnMysql(conn);

	return SQL_OK;
}

int MysqlConn::execSelect(const char* stmt, MysqlResult& resultSet)
{
	MYSQL* conn = getMysql();
	if (NULL == conn)
	{
		return SQL_FAIL;
	}

	int ret;
	ret = mysql_query(conn, stmt);
	if(ret)
	{
		if((ret = reconnect()) ==0)
		{
			conn = getMysql();
			ret = mysql_query(conn, stmt);
			if (ret)
			{
				ret = SQL_FAIL;
			}
		}		
		else
		{
			ret = SQL_CONNECT_ERROR;
		}
	}

	if(ret)
	{
		returnMysql(conn);
		return ret;
	}	

	MYSQL_RES* result = NULL;
	if ((result = mysql_store_result(conn)) == NULL)
	{
		returnMysql(conn);
		return SQL_FAIL;
	}

	resultSet.result(result);

	returnMysql(conn);

	return SQL_OK;
}

int MysqlConn::reconnect()
{
	mysql_close(conn_);
	conn_ = NULL;
	conn_ = mysql_init(NULL);
	if(!conn_)
	{
		return -1;
	}
	if (mysql_real_connect(conn_, 
		strlen(pconf_->ip_)> 0 ? pconf_->ip_: NULL,
		strlen(pconf_->user_) > 0 ? pconf_->user_ : NULL,
		strlen(pconf_->pswd_)> 0 ? pconf_->pswd_: NULL,
		NULL,
		pconf_->port_,
		strlen(pconf_->socket_) > 0 ? pconf_->socket_: NULL,
		0) == NULL)
	{
		return -1;
	}

	//select DB
	char use_db[MAX_MYSQL_CONTENT_LEN]={0};
	snprintf(use_db,MAX_MYSQL_CONTENT_LEN,
			"use %s ;",mysql_conf.mysqldb_conf.db_);
	int insert_id=0;
	int affect_num=0;
	int ret=mysql_conn.execUpdate(use_db,insert_id,affect_num);
	if(SQL_OK != ret)
	{
		printf("reconnect failed! switch to database[%s]failed!\n",use_db);
		return -1;
	}
	
	return 0;
}

int mysql_load_conf()
{
	Ul_confdata *pconf;
	pconf=ul_initconf(DEFAULT_CONF_NUM);
	if(!pconf)
	{
		printf("\n[Fatal Error] init config file error! \n");
		return -1;
	}
	memset(&mysql_conf,0,sizeof(mysql_conf));

	strcpy(mysql_conf.confpath,DEFAULT_CONFPATH_MYSQL);
	strcpy(mysql_conf.conffile,DEFAULT_CONFFILE_MYSQL);

	int ret=ul_readconf(mysql_conf.confpath,mysql_conf.conffile,pconf);
	if(ret<0){
		printf("\n[Fatal Error] load config file error! path[%s] file[%s] \n",mysql_conf.confpath,mysql_conf.conffile);
		return -1;
	}

	if (ul_getconfstr(pconf, "ip", mysql_conf.mysqldb_conf.ip_)) 
	{
		printf("Conf: mysql server ip: %s\n",mysql_conf.mysqldb_conf.ip_);
		printf("Conf: mysql server ip: %s", mysql_conf.mysqldb_conf.ip_);
	}
	else 
	{
		strcpy( mysql_conf.mysqldb_conf.ip_,DEFAULT_MYSQL_SERVER_IP);
		printf( "Conf: mysql server ip is: %s DEFAULT", DEFAULT_MYSQL_SERVER_IP);
	}

	if (ul_getconfstr(pconf, "user", mysql_conf.mysqldb_conf.user_)) 
	{
		printf( "Conf: mysql user: %s", mysql_conf.mysqldb_conf.user_);
	}
	else 
	{
		strcpy( mysql_conf.mysqldb_conf.user_,DEFAULT_MYSQL_USER);
		printf("Conf: mysql user is: %s DEFAULT", DEFAULT_MYSQL_USER);
	}

	if (ul_getconfstr(pconf, "pswd", mysql_conf.mysqldb_conf.pswd_)) 
	{
		printf("Conf: mysql pswd: %s", mysql_conf.mysqldb_conf.pswd_);
	}
	else 
	{
		strcpy( mysql_conf.mysqldb_conf.pswd_,DEFAULT_MYSQL_PSWD);
		printf("Conf: mysql pswd is: %s DEFAULT", DEFAULT_MYSQL_PSWD);
	}

	if (ul_getconfint(pconf, "port",&( mysql_conf.mysqldb_conf.port_))) 
	{
		printf("Conf: mysql port: %d", mysql_conf.mysqldb_conf.port_);
	}
	else 
	{
		mysql_conf.mysqldb_conf.port_ = DEFAULT_MYSQL_PORT;
		printf("Conf: mysql port is: %d DEFAULT", DEFAULT_MYSQL_PORT);
	}

	if (ul_getconfstr(pconf, "db", mysql_conf.mysqldb_conf.db_)) 
	{
		printf("Conf: mysql db: %s", mysql_conf.mysqldb_conf.db_);
	}
	else 
	{
		strcpy( mysql_conf.mysqldb_conf.db_,DEFAULT_MYSQL_DB);
		printf("Conf: mysql db is: %s DEFAULT", DEFAULT_MYSQL_DB);
	}

	ul_freeconf(pconf);
	pconf=NULL;

	return 0;
}

int mysql_conns_init()
{
	if(mysql_load_conf() != 0)
	{
		printf("\nmysql_load_conf failed!\n");
		exit(-1);
	}

	int ret=-1;
	ret = mysql_conn.initConn(mysql_conf.mysqldb_conf);
	if(SQL_OK != ret)
	{
		printf("sql conn init error.ip=[%s], port=[%d], user=[%s], pwd=[%s]",
				mysql_conf.mysqldb_conf.ip_,mysql_conf.mysqldb_conf.port_,
				mysql_conf.mysqldb_conf.user_,mysql_conf.mysqldb_conf.pswd_);
		exit(-1);
	}
	printf("connect to database[%s] successfuly!\n",mysql_conf.mysqldb_conf.db_);

	char use_db[MAX_MYSQL_CONTENT_LEN]={0};
	snprintf(use_db,MAX_MYSQL_CONTENT_LEN,
			"use %s ;",mysql_conf.mysqldb_conf.db_);

	int insert_id=0;
	int affect_num=0;
	ret=mysql_conn.execUpdate(use_db,insert_id,affect_num);
	if(SQL_OK != ret)
	{
		printf("use database[%s]failed!\n",use_db);
		exit(-1);
	}

	return 1;
}

