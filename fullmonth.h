#ifndef FULLMONTH_H
#define FULLMONTH_H

#include <QObject>
#include <QDate>

class FullMonth : public QObject
{
    Q_OBJECT
public:
    explicit FullMonth(QObject *parent = 0);

    void addDate(QDate);

    int days;
    int fill_month;
    int mentioned_month;

    int days_later;
signals:

public slots:

private:
    bool start;
    bool end;

    bool one_month;
    bool last_month;

    int days_buf;

    QDate old_date;
};

#endif // FULLMONTH_H
