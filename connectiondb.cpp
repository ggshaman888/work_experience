#include "connectiondb.h"
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QtSql>
ConnectionDB::ConnectionDB(QObject *parent) : QObject(parent)
{

}
bool ConnectionDB::createConnectionMYSQLite(QString path)
{
  QSqlDatabase dbsqlite = QSqlDatabase::addDatabase("QSQLITE", "dbsqlite");
  dbsqlite.setDatabaseName(path);
  qDebug() << dbsqlite.drivers();
  if (!dbsqlite.open()) {
    qDebug() << "Cannot open database: " << dbsqlite.lastError();
    return false;
  }
  return true;
}
