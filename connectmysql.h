#ifndef CONNECTMYSQL_H
#define CONNECTMYSQL_H

#include <qsqldatabase.h>
#include <qsqlerror.h>

class connectmysql
{
public:
    connectmysql();
    void close();
    QSqlDatabase* getdb() {return &db;};
private:
    QSqlDatabase db;
};

#endif // CONNECTMYSQL_H
