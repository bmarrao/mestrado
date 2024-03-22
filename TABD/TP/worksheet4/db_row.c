#include "Yap/YapInterface.h"
#include <stdio.h>
#include <string.h>
#include <mysql.h>

typedef struct 
{
    YAP_Term pair; /* the next solution */
} db_row_data_type;

db_row_data_type *db_row_data;

int init_db_row (void)
{

    YAP_PRESERVE_DATA(db_row_data,db_row_data_type);
    
}
int db_row(void)
{
    MYSQL_RES *result = (MYSQL_RES *) YAP_IntOfTerm(YAP_ARG1);
    MYSQL_ROW row;
    int arity = mysql_num_fields(result);
    if ((row = mysql_fetch_row(result)) != NULL)
    {
        YAP_Term head, list = YAP_ARG2 ;
        //int arity =  YAP_ArityOfFunctor
        for (int i =0 ; i < arity; i++)
        {
            head = YAP_HeadOfTerm(list);
            list = YAP_TailOfTerm(list);
            if (!YAP_Unify(head, YAP_MkAtomTerm(row[i])))
            {
                return false;
            }
        }
        return true;
    }
    mysql_free_result(res_set);
    YAP_cut_fail();
}

void init_db_row()
{
    YAP_UserBackCPredicate("db_row", init_db_row, db_row, 3, 1);
}