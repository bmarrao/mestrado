#include <stdio.h>
#include <mysql.h>
void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}


int main(int argc, char **argv)
{
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
    if (mysql_query(con, "DROP TABLE IF EXISTS Cars"))
    {
        finish_with_error(con);
    }
    if (mysql_query(con, "CREATE TABLE Cars(Id INT, Name TEXT, Price INT)"))
    {
        finish_with_error(con);
    }
    if (mysql_query(con, "INSERT INTO Cars VALUES(1,'Audi',52642)"))
    {
        finish_with_error(con);
    }
    if (mysql_query(con, "INSERT INTO Cars VALUES(2,'Mercedes',57127)"))
    {
        finish_with_error(con);
    }
    mysql_close(con);
    exit(0);
}