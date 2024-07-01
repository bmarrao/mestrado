#include "Yap/YapInterface.h"
static int start_n100(void);
static int continue_n100(void);
typedef struct 
{
    YAP_Term next_solution; /* the next solution */
} n100_data_type;

n100_data_type *n100_data;

static int start_n100(void)
{
    YAP_Term t = YAP_ARG1;
    YAP_PRESERVE_DATA(n100_data,n100_data_type);
    if(YAP_IsVarTerm(t)) 
    {
        n100_data->next_solution = YAP_MkIntTerm(0);
        return continue_n100();
    }
    if(!YAP_IsIntTerm(t) || YAP_IntOfTerm(t)<0 || YAP_IntOfTerm(t)>100) 
    {
        YAP_cut_fail();
    } 
    else 
    {
        YAP_cut_succeed();
    }
}

static int continue_n100(void)
{
    int n;
    YAP_Term t;
    YAP_Term sol = YAP_ARG1;
    YAP_PRESERVED_DATA(n100_data,n100_data_type);
    n = YAP_IntOfTerm(n100_data->next_solution);
    if( n == 100) 
    {
        t = YAP_MkIntTerm(n);
        YAP_Unify(sol,t);
        YAP_cut_succeed();
    }
    else 
        {
        YAP_Unify(sol,n100_data->next_solution);
        n100_data->next_solution = YAP_MkIntTerm(n+1);
        return(TRUE);
    }
}

init_n100(void)
{
    YAP_UserBackCPredicate("n100", start_n100, continue_n100, 1, 1);
}