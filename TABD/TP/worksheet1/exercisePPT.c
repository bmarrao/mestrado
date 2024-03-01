#include <stdio.h>
#include <mysql.h>
#include <string.h>
void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}
char *type_info(enum enum_field_types type) {
    switch (type) {
        case MYSQL_TYPE_VARCHAR:
            return "VARCHAR";
        case MYSQL_TYPE_NEWDECIMAL:
            return "NEWDECIMAL";
        case MYSQL_TYPE_LONG:
            return "LONG";
        case MYSQL_TYPE_VAR_STRING:
            return "VAR_STRING";
            //...several cases omitted see field_types.h
        default:
            return "UNKNOWN";
    }
}
void printRow(MYSQL_FIELD **fields,int num_fields)
{   
    printf("+");
    for (int i=0; i < num_fields; i++)
    {
        for (int j = 0 ; j <fields[i]->max_length+4; j++)
        {
            printf("-");
        }
        printf("+");
    }
    printf("\n");
}
void printData(MYSQL_FIELD **fields, MYSQL_RES *result ,int num_fields)
{
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        printf("|");
        for (int i = 0; i < num_fields; i++)
        {
            char* name = row[i] ? row[i] : "NULL";
            printf(" ");
            if (strcmp(type_info(fields[i]->type),"LONG") == 0)
            {
                for (int j = strlen(name); j < fields[i]->max_length+2; j++)
                {
                    printf(" ");
                }
                printf("%s", name);
                printf(" ");
            }
            else 
            {
                printf("%s", name);  
                for (int j = strlen(name)+1; j < fields[i]->max_length+4; j++)
                {
                    printf(" ");
                }  
            }
            printf("|");
            }
        printf("\n");
    }
}
void printNames(MYSQL_FIELD **fields,int num_fields)
{   
    printf("|");
    for (int i=0; i < num_fields; i++)
    {
        char* name = fields[i]->name;
        printf(" ");

        printf("%s", name);

        for (int j = strlen(name)+1; j < fields[i]->max_length+4; j++)
        {
            printf(" ");
        }
        printf("|");
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    MYSQL *con = mysql_init(NULL);
    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }
    if (mysql_real_connect(con, "localhost", "newuser", "Tabd2324!",
    "testdb", 0, NULL, 0) == NULL)    
    {
        finish_with_error(con);
    }
    if (mysql_query(con, "SELECT * FROM Fib"))
    {
        finish_with_error(con);
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL)
    {
        finish_with_error(con);
    }
    int num_fields = mysql_num_fields(result);
    int size ;
    int maxSizeField [num_fields];
    MYSQL_FIELD *fields[num_fields];
    for (int i=0; i < num_fields; i++)
    {
        fields[i] = mysql_fetch_field(result);
        maxSizeField [i]= fields[i]->max_length ;
    }


    for (int i=0; i < num_fields; i++)
    {
        printf("%s\n",fields[i]->name);
    }

    printRow(fields, num_fields);
    printNames(fields,num_fields);
    printRow(fields, num_fields);

    printData(fields,result, num_fields);
    
    printRow(fields,num_fields);
    mysql_free_result(result);
    mysql_close(con);
    exit(0);
}

