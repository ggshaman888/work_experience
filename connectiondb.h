#ifndef CONNECTIONDB_H
#define CONNECTIONDB_H

#include <QObject>

class ConnectionDB : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionDB(QObject *parent = 0);
    bool createConnectionMYSQLite(QString);

signals:

public slots:
};

#endif // CONNECTIONDB_H
