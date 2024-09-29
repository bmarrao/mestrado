#include <stdio.h>
#include <mysql.h>
void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

int fibonacci(int n) 
{
    int a = 0, b = 1, c, i;
    if (n == 0)
        return a;
    for (i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
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
    if (mysql_query(con, "DROP TABLE IF EXISTS Fib"))
    {
        finish_with_error(con);
    }
    if (mysql_query(con, "CREATE TABLE Fib(n INT, fib INT)"))
    {
        finish_with_error(con);
    }
    for (int i = 1 ;i <= 20 ; i++)
    {
        char query [50]; 

        sprintf(query, "INSERT INTO Fib VALUES(%d, %d)",  i, fibonacci(i));
        if (mysql_query(con, query))
        {
            finish_with_error(con);
        }
    }
    mysql_close(con);
    exit(0);
}