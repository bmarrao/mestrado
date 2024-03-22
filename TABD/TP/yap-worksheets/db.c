#include "Yap/YapInterface.h"
#include <stdio.h>
#include <string.h>
#include <mysql.h>

typedef struct 
{
    MYSQL_RES *result;
    YAP_Term pair; /* the next solution */
    YAP_Term root;
    int arity;
} db_row_data_type;

db_row_data_type *db_row_data;

int init_db_row (void)
{

    YAP_PRESERVE_DATA(db_row_data,db_row_data_type);
    db_row_data->result = (MYSQL_RES *) YAP_IntOfTerm(YAP_ARG1);
    db_row_data->pair = YAP_MkNewPairTerm();
    db_row_data->root = db_row_data->pair;
    db_row_data->arity = mysql_num_fields(db_row_data->result);
    return db_row();
}
int db_row(void)
{
    MYSQL_ROW row;
    if ((row = mysql_fetch_row(db_row_data->result)) != NULL)
    {
        YAP_Term list = YAP_HeadOfTerm(db_row_data->pair);
        //int arity =  YAP_ArityOfFunctor
        for (int i =0 ; i < db_row_data->arity; i++)
        {
            YAP_Term head = YAP_HeadOfTerm(list);
            list = YAP_TailOfTerm(list);
            if (!YAP_Unify(head, YAP_MkAtomTerm(row[i])))
            {
                return false;
            }
        }
        db_row_data->pair = YAP_TailOfTerm(db_row_data->pair);
        return(TRUE);
    }
    else 
    {
        
        if (!YAP_Unify(YAP_ARG2, db_row_data->root))
        {
            return false;
        }
        mysql_free_result(db_row_data->result);
        YAP_cut_succeed();

    }
}

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

int db_disconnect(void)
{
    YAP_Term arg_conn = YAP_ARG1;
    MYSQL *conn  = (MYSQL *) YAP_IntOfTerm(arg_conn);
    mysql_close(conn);
    return true;

}
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

void init_predicates()
{
    YAP_UserCPredicate("db_connect",db_connect,5);
    YAP_UserCPredicate("db_disconnect",db_disconnect,1);
    YAP_UserCPredicate("db_query",db_query,3);
    YAP_UserBackCPredicate("db_row", init_db_row, db_row, 2, 1);

}


