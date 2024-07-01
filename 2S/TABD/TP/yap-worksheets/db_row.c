#include "Yap/YapInterface.h"
#include <stdio.h>
#include <string.h>
#include <mysql.h>

typedef struct 
{
    MYSQL_RES *result;
    YAP_Term pair; /* the next solution */
    int arity;
} db_row_data_type;

db_row_data_type *db_row_data;

int init_db_row (void)
{

    YAP_PRESERVE_DATA(db_row_data,db_row_data_type);
    db_row_data->result = (MYSQL_RES *) YAP_IntOfTerm(YAP_ARG1);
    db_row_data->pair = YAP_ARG2;
    db_row_data->arity = mysql_num_fields(db_row_data->result);
    return db_row();
}
static int db_row(void)
{
    MYSQL_ROW row;
    if ((row = mysql_fetch_row(db_row_data->result)) != NULL)
    {
        YAP_Term list = YAP_HeadOfTerm(db_row_data->pair)
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
        db_row_data->pair = YAP_TailOfTerm(db_row_data->pair);
        return(TRUE);
    }
    else 
    {
        
        mysql_free_result(res_set);
        YAP_cut_succeed();

    }
}

void init_db_row()
{
    YAP_UserBackCPredicate("db_row", init_db_row, db_row, 3, 1);
}