#ifndef _MYSQL_CONN_H_
#define _MYSQL_CONN_H_

#include "mysql.h"


#define QRY_DB_TIMEOUT 5
#define MAX_STRLEN	512

#define DEFAULT_CONF_NUM 30
#define DEFAULT_CONFPATH_MYSQL "./conf/"
#define DEFAULT_CONFFILE_MYSQL "mysql.conf"
#define DEFAULT_MYSQL_SERVER_IP "localhost"
#define DEFAULT_MYSQL_USER "root"
#define DEFAULT_MYSQL_PSWD "123456"
#define DEFAULT_MYSQL_PORT 19809
#define DEFAULT_MYSQL_DB "test_tb"
#define MAX_MYSQL_DIR_LEN 256
#define MAX_MYSQL_CONTENT_LEN 102400
typedef unsigned long long uint64;
typedef unsigned int uint32;



#pragma pack(1)
typedef enum
{
	SQL_OK	 = 0	,
	SQL_FAIL = -1	,
	SQL_CONNECT_ERROR	=	SQL_FAIL-1	,

}ENUM_SQL_ERROR;

struct DBConf
{
	DBConf()
	{
		port_=0;
		total_db_num_=0;
		tbl_num_per_db_=0;
        db_[0] = '\0';
        tbl_[0] = '\0';
        ip_[0] = '\0';
        user_[0] = '\0';
        pswd_[0] = '\0';
        socket_[0] = '\0';
	}
	
	int port_;
	int total_db_num_;//总共的db数,包括别的机器上
	int tbl_num_per_db_;

	char db_[MAX_STRLEN];//db前缀
	char tbl_[MAX_STRLEN];//tbl前缀
	char ip_[MAX_STRLEN];
	char user_[MAX_STRLEN];
	char pswd_[MAX_STRLEN];
	char socket_[MAX_STRLEN];
};
#pragma pack()

class MysqlResult
{
protected:
    MYSQL_RES* result_;
	
public:
    MysqlResult()
    {
        result_ = NULL;
    }

    MysqlResult(MYSQL_RES* resultSet)
    {
        result_ = resultSet;
    }
    ~MysqlResult()
    {
        if (result_ != NULL)
        {
            mysql_free_result(result_);
            result_ = NULL;
        }

    }
    void result(MYSQL_RES* rst) 
    {
        if (result_ != NULL)
        {
            mysql_free_result(result_);
            result_ = NULL;
        }

        result_ = rst;
    }
    MYSQL_RES* result() { return result_; }
};

// the database connection class
class MysqlConn
{
public:
	MysqlConn();
	~MysqlConn();

	int initConn(DBConf &db_conf);
	void finiConn();

	MYSQL* getMysql();
	void returnMysql(MYSQL* mysql);

	// execute update sql statement(update, insert, delete)
	int execUpdate(const char* stmt, int& insertID, int& affected);

	// execute select sql statement
	int execSelect(const char* stmt, MysqlResult& resultSet);

private:
	int reconnect();
	DBConf *pconf_;    
	MYSQL* conn_;

};

#define KNULL(x) (x==NULL ? "0":x)

typedef struct _Mysql_Conf_t
{
	char confpath[255];
	char conffile[255];
	struct DBConf mysqldb_conf;
}Mysql_Conf_t;

extern MysqlConn mysql_conn;
extern Mysql_Conf_t mysql_conf;

int mysql_load_conf();
int mysql_conns_init();

#endif 
