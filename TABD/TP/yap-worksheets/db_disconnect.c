#include "Yap/YapInterface.h"
#include <stdio.h>
#include <string.h>
#include <mysql.h>
int db_close(void)
{
    YAP_Term arg_conn = YAP_ARG1;
    MYSQL *conn  = (MYSQL *) YAP_IntOfTerm(arg_conn);
    mysql_close(conn);
    return true;

}

void init_db_disconnect()
{
    YAP_UserCPredicate("db_disconnect",db_disconnect,1);
}