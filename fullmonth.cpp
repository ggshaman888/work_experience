#include "fullmonth.h"

FullMonth::FullMonth(QObject *parent) : QObject(parent)
{
    fill_month = 0;
    mentioned_month = 0;
    start = false;
    end = false;
    one_month = false;
    last_month = false;
    days = 0;
    days_buf = 0;
    days_later = 0;
}

void FullMonth::addDate(QDate date)
{
    if(old_date.isNull())
    {
        old_date = date;
        mentioned_month++;

        if(date.day()==1)
            one_month = false;
        else
            one_month = true;

    }

    if(one_month)
        days_later++;

    if(old_date.day()>date.day())
       end = true;

    if(start)
    {
        if(end)
        {
            fill_month++;
            start = false;
            end = false;
            days_buf = 0;
        }
    }


    days_buf++;
    if(date.day()==1)
       start = true;


    if(old_date.month()!=date.month())
    {
        mentioned_month++;
        if(one_month)
            days_later = days_buf;
        one_month = false;
    }

    days = days_buf + days_later;
    old_date = date;

}
