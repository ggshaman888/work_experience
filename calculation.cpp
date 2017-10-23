#include "calculation.h"
#include <QStringList>
#include <QDateTime>
#include <calendar.h>
#include <fullmonth.h>

Calculation::Calculation(QObject *parent) : QObject(parent)
{
    c = new Calendar();
    clr();
}

void Calculation::setValue(QString start, QString end, QString minus)
{
    clr();
   // c->fillCalendar();

    FullMonth *m = new FullMonth();

    if(minus=="-") minus_is = true;
    else minus_is = false;

    initVal(start, end);

    QDate s, e;
    s = QDate::fromString(start, "dd.MM.yyyy");
    e = QDate::fromString(end, "dd.MM.yyyy");
    e = e.addDays(1);

    while(s<e)
    {
        m->addDate(s);

        //countReadMentioned(s);
        s = s.addDays(1);
    }
    if(m->days_later<31)
        if(m->days_later>1)
            m->fill_month--;

    if(s.month()==1)
        if(s.day()==1)
            day_is--;

    month_is = m->fill_month + m->days/30;
    day_is = m->days%30;



    month_buf = month_is%12;
    year_is = (month_is-month_buf)/12;
    month_is = month_buf;

//    countMonthFull();
//    countDays();
//    countYears();

    day_is_l << day_is;
    month_l << month_is;
    years_l << year_is;
    minus_l << minus_is;
}

QString Calculation::calculate()
{
    year_is = 0;
    month_is = 0;
    day_is = 0;

    int size = day_is_l.size();
    for(int i=0;i<size;i++)
    {
        if(minus_l.at(i))
        {
            year_is += -years_l.at(i);
            day_is += -day_is_l.at(i);
            month_is += -month_l.at(i);
        }
        else
        {
            year_is += years_l.at(i);
            day_is += day_is_l.at(i);
            month_is += month_l.at(i);
        }
    }
    if(day_is>30)
    {
        int x = day_is%30;
        month_is += (day_is-x)/30;
        day_is = x;
    }
    if(month_is>12)
    {
        int x = month_is%12;
        year_is += (month_is-x)/12;
        month_is = x;
    }

    return QString(QString::number(day_is) + "." +
                   QString::number(month_is) + "." +
                   QString::number(year_is));
}



void Calculation::initVal(QString start, QString end)
{

    QStringList year_s = start.split(".");
    QStringList year_e = end.split(".");

    d_start = clrZero(year_s.at(0));
    m_start = clrZero(year_s.at(1));
    y_start = clrZero(year_s.at(2));

    d_end = clrZero(year_e.at(0));
    m_end = clrZero(year_e.at(1));
    y_end = clrZero(year_e.at(2));
}

void Calculation::countMonthFull()
{
    int start = y_start;
    int end = y_end+1;

    for(;start<end;start++)
    {
        c->readYear(start);
        int size = c->day_l.size();
        for(int i=0;i<size;i++)
            make(c->day_l.at(i));
    }

}

void Calculation::countDays()
{

    if(count_month_part==2)
    {
        day_is = d_end - d_start;
        if(day_is<0)
            day_is = 30-day_is;
    }
    if(count_month_part==1)
    {
        if(d_start>d_end)
            day_is = d_start;
        else
            day_is = d_end;
    }
    if(count_month_part==0)
        day_is = 0;

}

void Calculation::countYears()
{
    month_is = count_month_full%12;
    year_is = (count_month_full-month_is)/12;
}

void Calculation::make(int day_is)
{
    if(day_is == 1)
        start_month_day = true;

    if(day_is<day_operation)
        end_month_day = true;

    day_operation = day_is;

    if(end_month_day)
    {
        if(start_month_day)
            count_month_full++;
        else
            count_month_part++;
    }
    end_month_day = false;
}

void Calculation::clr()
{
    start_month_day = 0;
    end_month_day = 0;
    day_operation = 0;
    count_month_part = 0;
    count_month_full = 0;
    day_is = 0;
    year_is = 0;
    month_is = 0;
}

int Calculation::clrZero(QString number)
{
    if(number[0]=='0')
        clrZero(number.remove(0,1));

    return number.toInt();
}

void Calculation::countReadMentioned(QDate date)
{
    //countFullMonth(date);
    if(month_buf!=date.month())
        month_is++;
    month_buf=date.month();


    if(one_month==0)
        one_month = date.month();

    if(one_month == date.month())
        day_is++;
    else
    {
        if(last_month==0)
            last_month = date.month();
    }

    if(last_month == date.month())
        day_is++;
}
