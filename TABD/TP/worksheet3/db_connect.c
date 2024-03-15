#include "Yap/YapInterface.h"
#include <stdio.h>
#include <string.h>
#include <mysql.h>
int db_connect(void)
{
    YAP_Term arg_host = YAP_ARG1;
    YAP_Term arg_user = YAP_ARG2;
    YAP_Term arg_passwd = YAP_ARG3;
    YAP_Term arg_database = YAP_ARG4;
    YAP_Term arg_conn = YAP_ARG5;
    MYSQL *conn ;
    char *host = YAP_AtomName(YAP_AtomOfTerm(arg_host));
    char *user = YAP_AtomName(YAP_AtomOfTerm(arg_user));
    char *passwd = YAP_AtomName(YAP_AtomOfTerm(arg_passwd));
    char *database = YAP_AtomName(YAP_AtomOfTerm(arg_database));
    conn = mysql_init(NULL);
    if (conn == NULL)
    {
        printf("Errei no INit");
        return FALSE ;
    }
    if (mysql_real_connect(conn, host, user, passwd,
    database, 0, NULL, 0) == NULL)    
    {
        printf("Erro no connect");
        return false ;
    }
    if (!YAP_Unify(arg_conn, YAP_MkIntTerm((long) conn)))
    {
        return false;
    }
    return true;

}

void init_db_connect()
{
    YAP_UserCPredicate("db_connect",db_connect,5);
}