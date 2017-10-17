#include "calendar.h"
#include <QSqlQuery>
#include <QDate>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QSqlError>

Calendar::Calendar(QObject *parent) : QObject(parent)
{
    clr();
}

void Calendar::addToCalendar()
{
    QSqlQuery query(QSqlDatabase::database("dbsqlite"));

    query.prepare("INSERT INTO calendar (year, month, day, status, mod) "
          "VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(year);
    query.addBindValue(month);
    query.addBindValue(day);
    query.addBindValue(status);
    query.addBindValue(mod);
    query.exec();
}

void Calendar::readYear(int is_year)
{
    clr();
    QSqlQuery query(QSqlDatabase::database("dbsqlite"));
    QString str = "SELECT * FROM calendar WHERE year = '%1';";
    str = str.arg(QString::number(is_year));

    query.exec(str);
    qDebug() << query.lastError() << str;

    while(query.next())
    {
        year_l << query.value(1).toInt();
        month_l << query.value(2).toInt();
        day_l << query.value(3).toInt();
        status_l << query.value(4).toString();
        mod_l << query.value(5).toString();
    }
}

void Calendar::readDay(int is_day, int is_month, int is_year)
{
    QSqlQuery query(QSqlDatabase::database("dbsqlite"));
    QString str = "SELECT * FROM calendar WHERE year = '%1' AND "
                  "month = '%2 AND day = '%3';";
    str.arg(is_year).arg(is_month).arg(is_day);

    query.exec(str);
    qDebug() << query.lastError();

    query.next();
    year = query.value(1).toInt();
    month = query.value(2).toInt();
    day = query.value(3).toInt();
    status = query.value(4).toString();
    mod = query.value(5).toString();
}

bool Calendar::workedDay(QDate date)
{
    if(holiday.isEmpty())
        readCalendarCsv();

    return workedIs(date, notDB(date.year()));
}

void Calendar::fillCalendar()
{
    clrCalendar();
    QSqlQuery query(QSqlDatabase::database("dbsqlite"));

    QDate x, end; x.setDate(1901, 1, 1);
    end = QDate::currentDate();

    while(x<end)
    {
        query.prepare("INSERT INTO calendar (year, month, day, status) VALUES (?, ?, ?, ?)");
        query.bindValue(0, x.year());
        query.bindValue(1, x.month());
        query.bindValue(2, x.day());
        query.bindValue(3, workedDay(x));

        query.exec();
      //  qDebug() << query.lastError();
        x = x.addDays(1);
    }
}

bool Calendar::workedIs(QDate date, bool no)
{
    if(no)
        date.setDate(minYearsWorked(), date.month(), date.day());

    for(int i=0;i<holiday.size();i++)
    {
        if(holiday.at(i)==date)
        {
            if(date.toString("ddd")=="sun")
                return true;
            else
                if(date.toString("ddd")=="sat")
                    return true;
            return false;
        }
    }
    return true;
}

void Calendar::readCalendarCsv()
{
    QString str;
    QString p = "/home/alexandr/Documents/My creation/My program/work_experience/work_experience/calendar.csv";
    QFile f(p);
    if(f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&f);
        while(!f.atEnd())
            toCsvStr(in.readLine());
    }
}

void Calendar::toCsvStr(QString str)
{
    QStringList str_l, buf_l;
    QString buf;
    int year, month;
    if(str.indexOf("Г")!=0)
    {
        str_l = str.split(",\"");
        year = QString(str_l.at(0)).toInt();

        for(int i=1;i<str_l.size();i++)
        {
            month = i; buf = str_l.at(i);
            buf.replace("\"", ""); buf.replace("*", "");
            buf_l = buf.split(",");

            for(int j=0;j<buf_l.size();j++)
            {
                buf = buf_l.at(j);
                QDate z; z.setDate(year, month, QString(buf.at(j)).toInt());
                holiday << z;
            }
        }
    }
}

void Calendar::clr()
{
    year = 0;
    month = 0;
    day = 0;
    status = "";
    mod = "";

    year_l.clear();
    month_l.clear();
    day_l.clear();
    status_l.clear();
    mod_l.clear();
}

bool Calendar::notDB(int year)
{
    for(int i=0;i<holiday.size();i++)
        if(holiday.at(i).day()==year)
            return false;
    return true;
}

int Calendar::minYearsWorked()
{
    int x = 9999;
    for(int i=0;i<holiday.size();i++)
        if(holiday.at(i).year()<x)
            x=holiday.at(i).year();
    return x;
}

void Calendar::clrCalendar()
{
    QSqlQuery query(QSqlDatabase::database("dbsqlite"));
    QString str = "DELETE FROM calendar;";

    qDebug() << query.exec(str) << str;
    qDebug() << query.lastError();
}
