#include <stdio.h>
#include <mysql.h>
void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

char *type_info(enum enum_field_types type) 
{
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
//yap -l datalogDinasty.yap
void writeSelectToFile(MYSQL_FIELD **fields,int num_fields,MYSQL_RES *result, char *name, File *file )
[
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        fprintf(file,"%s(",name);
        for (int i = 0; i < num_fields-1; i++)
        {
            
            char* name = row[i] ? row[i] : "NULL";
            if (strcmp(type_info(fields[i]->type),"LONG") == 0)
            {
                fprintf(file,"%s",name);
            }
            else 
            {
                fprintf("'%s'", name);  
            }
            fprintf(file,",");
        }
         if (strcmp(type_info(fields[i]->type),"LONG") == 0)
        {
            fprintf(file,"%s",name);
        }
        else 
        {
            fprintf(file,"'%s'", name);  
            
        }
        printf(").\n");
    }
]


int main(int argc, char **argv)
{
    FILE *file;
    file = fopen("ws2.prolog", "w");
    if (file == NULL) 
    {
        printf("Error opening the file.\n");
        return 1; // Return an error code
    }
    MYSQL *con = mysql_init(NULL);
    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }
    if (mysql_real_connect(con, "localhost", "newuser", "Tabd2324!",
    "testdb", 0, NULL, 0) == NULL)    
    {
        finish_with_error(con);
    }
    if (mysql_query(con, "SELECT * FROM person"))
    {
        finish_with_error(con);
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL)
    {
        finish_with_error(con);
    }
    int num_fields = mysql_num_fields(result);
    MYSQL_FIELD *fields[num_fields];
    for (int i=0; i < num_fields; i++)
    {
        fields[i] = mysql_fetch_field(result);
    }
    writeSelectToFile(fields,num_fields,result, "person",file );

    if (mysql_query(con, "SELECT * FROM marriage"))
    {
        finish_with_error(con);
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL)
    {
        finish_with_error(con);
    }
    int num_fields2 = mysql_num_fields(result);
    MYSQL_FIELD *fields2[num_fields];
    for (int i=0; i < num_fields; i++)
    {
        fields2[i] = mysql_fetch_field(result);
    }
    writeSelectToFile(fields2,num_fields2,result, "marriage",file );

    if (mysql_query(con, "SELECT * FROM parent"))
    {
        finish_with_error(con);
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL)
    {
        finish_with_error(con);
    }
    int num_fields3 = mysql_num_fields(result);
    MYSQL_FIELD *fields3[num_fields3];
    for (int i=0; i < num_fields; i++)
    {
        fields[i] = mysql_fetch_field(result);
    }
    writeSelectToFile(fields3,num_fields3,result, "parent",file );
    mysql_close(con);
    exit(0);
}