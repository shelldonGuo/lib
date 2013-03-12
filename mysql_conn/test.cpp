#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "mysql_conn.h"

#define MAX_CONTENT_LEN 1024

int main()
{
    mysql_conns_init();

    int insert_id,affect_num;
    char sql_query[MAX_CONTENT_LEN];
    MysqlResult my_rst;
    MYSQL_ROW row;
    memset(sql_query,0,sizeof(sql_query));

    snprintf(sql_query,MAX_CONTENT_LEN, 
            "insert into rts_buf (objsign,objurl,fromurl) values (%llu,'%s','%s');", 
            12345,"http://1.jpg","http://1.html");

    int ret=mysql_conn.execUpdate(sql_query,insert_id,affect_num);
    if(SQL_FAIL == ret)
    {
        MYSQL* conn = mysql_conn.getMysql();
        printf("insert item failed! sql:[%s] msg:[%s]",sql_query,mysql_error(conn));
        return -1;
    }


    snprintf(sql_query,MAX_CONTENT_LEN, 
            "select objurl,fromurl from  rts_buf where objsign=%llu;", 
            12345);

    ret=mysql_conn.execSelect(sql_query,my_rst);
    if(SQL_FAIL == ret)
    {
        MYSQL* conn = mysql_conn.getMysql();
        printf("select item failed! sql:[%s] msg:[%s]",sql_query,mysql_error(conn));
        return -1;
    }

    if(NULL != my_rst.result() && NULL!=(row=mysql_fetch_row(my_rst.result())))
    {
        printf("objurl=%s\n",row[0]);

    }
    else{
        MYSQL* conn = mysql_conn.getMysql();
        printf("sql result error! sql:[%s] msg:[%s]",sql_query,mysql_error(conn));
        return -1;
    }

    return 0;

}

















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
