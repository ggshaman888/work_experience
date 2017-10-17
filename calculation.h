#ifndef CALCULATION_H
#define CALCULATION_H

#include <QObject>
#include <calendar.h>
#include <QDate>

class Calculation : public QObject
{
    Q_OBJECT
public:
    explicit Calculation(QObject *parent = 0);
    void setValue(QString, QString, QString);
    QString calculate();


signals:

public slots:

private:
    QList<int> day_is_l;
    QList<int> month_l;
    QList<int> years_l;
    QList<bool> minus_l;

    int day_c;
    int month_c;
    int yars_c;

    bool minus_is;

    int d_start;
    int m_start;
    int y_start;

    int d_end;
    int m_end;
    int y_end;

    void initVal(QString,QString);
    void countMonthFull();
    void countDays();
    void countYears();
    void make(int);
    void clr();

    int day_is;
    int year_is;
    int month_is;
    int day_buf;
    int year_buf;
    int month_buf;
    int start_month_day;
    int end_month_day;
    int day_operation;
    int count_month_full;
    int count_month_part;

    int clrZero(QString);

    int one_month;
    int last_month;

    void countReadMentioned(QDate);

    Calendar *c;
};

#endif // CALCULATION_H
