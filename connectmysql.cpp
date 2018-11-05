#include "connectmysql.h"

#include <qdebug.h>
connectmysql::connectmysql()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("ec2-54-244-76-188.us-west-2.compute.amazonaws.com");
    db.setUserName("arash");
    db.setPassword("8091921.z.Z");
    db.setDatabaseName("BioRetentionData");
    if( !db.open() )
      {
        qDebug() << db.lastError();
        qFatal( "Failed to connect." );
      }

      qDebug( "Connected!" );

}

void connectmysql::close()
{
    db.close();
}
