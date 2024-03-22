#include "Yap/YapInterface.h"
#include <stdio.h>
#include <string.h>
#include <mysql.h>
int db_query(void)
{
    MYSQL *conn = (MYSQL*) YAP_IntOfTerm(YAP_ARG1);
    char * query = YAP_AtomName(YAP_AtomOfTerm(YAP_ARG2));

    if (conn == NULL)
    {
        printf("Erro no conn");
        return false;
    }
    if (mysql_query(conn, query))
    {
        printf("Erro na query");
        return false ;
    }
    MYSQL_RES *result = mysql_store_result(conn);
    return (YAP_Unify(YAP_ARG3,YAP_MkIntTerm((int)result)));
}

void init_db_query()
{
    YAP_UserCPredicate("db_query",db_query,3);
}