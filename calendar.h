#ifndef CALENDAR_H
#define CALENDAR_H

#include <QObject>
#include <QDate>
#include <QStringList>
class Calendar : public QObject
{
    Q_OBJECT
public:
    explicit Calendar(QObject *parent = 0);

    void addToCalendar();
    void readYear(int);
    void readDay(int, int, int);

    bool workedDay(QDate);
    void fillCalendar();

    int year;
    int month;
    int day;
    QString status;
    QString mod;

    QList<int> year_l;
    QList<int> month_l;
    QList<int> day_l;
    QStringList status_l;
    QStringList mod_l;

    QList<QDate> holiday;

signals:

public slots:
private:
    bool workedIs(QDate, bool);
    void readCalendarCsv();
    void toCsvStr(QString);
    void clr();
    bool notDB(int);
    int minYearsWorked();
    void clrCalendar();
};

#endif // CALENDAR_H
