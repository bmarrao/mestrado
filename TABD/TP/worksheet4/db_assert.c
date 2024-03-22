#include "Yap/YapInterface.h"
#include <stdio.h>
#include <string.h>
#include <mysql.h>
int db_close(void)
{
    int arity;
    char query[100];
    YAP_Functor f_pred, f_assert ;
    YAP_Term t_pred , *t_args, t_assert;
    MYSQL *conn  = (MYSQL *) YAP_IntOfTerm(YAP_ARG1);
    sprintf(query, "SELECT * from %s", YAP_AtomName(YAP_AtomOfTerm(YAP_ARG2)));
    mysql_query(conn,query);
    MYSQL_RES *result = mysql_store_result(conn);
    arity = mysql_num_fields(res_set);
    YAP_Term n_args[arity];
    f_pred = YAP_MkFunctor(YAP_AtomOfTerm(YAP_ARG3),arity);
    f_assert =  YAP_MkFunctior(YAP_LookupAtom("assert"),1);
    while((row = mysql_fetch_row(result)) != NULL)
    {

        for (int i =0 ; i < arity ; i++)
        {

        }
        t_pred = YAP_MkApplTerm(f_pred, arity,t_args);
        t_assert = YAP_MkApplTerm(f_assert, 1, &t_pred);
        YAP_CallProlog(t_assert);
    }
    mysql_free_result(res_set);
    return TRUE;

}

void init_db_disconnect()
{
    YAP_UserCPredicate("db_assert",db_assert,3);
}