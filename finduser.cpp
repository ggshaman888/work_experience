#include "finduser.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
FindUser::FindUser(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(refreshInfo()), this, SLOT(refresh()));
}

void FindUser::id_list_update()
{
    QSet <int> b;
    id_list.clear();
    b = id_last_name.toSet() & id_name.toSet();
    b.intersect(id_patronymic.toSet());
    b.intersect(id_birthday.toSet());

    if(b.isEmpty())
        emit refreshInfo();
    id_list = b.toList();
}

void FindUser::readDB()
{
    QSqlQuery query(QSqlDatabase::database("dbsqlite"));
    QString str = "SELECT * FROM user;";

    qDebug() << query.exec(str) << str;
    qDebug() << query.lastError();

    while (query.next())
    {
         id << query.value(0).toInt();
         last_nameL << query.value(1).toString();
         nameL << query.value(2).toString();
         patronymicL << query.value(3).toString();
         birthdayL << query.value(4).toString();
    }
}

void FindUser::findLastName()
{
    if(!isU.last_name.isEmpty())
    {
        QStringList z = last_nameL.filter(isU.last_name);
        if(!z.isEmpty())
        {
            x = last_nameL.indexOf(z.at(0));
            inU.last_name = last_nameL.at(x);
        }
        else
            inU.last_name = "";
    }
    //findName();

}

void FindUser::findName()
{
    if(!isU.name.isEmpty())
    {
        QStringList z = nameL.filter(isU.name);
        if(!z.isEmpty())
        {
            if(x==-1)
                x = nameL.indexOf(z.at(0));
            inU.name = nameL.at(x);
        }
        else
            inU.name = "";
    }
}

void FindUser::findPatronymic()
{
    if(!isU.patronymic.isEmpty())
    {
        QStringList z = patronymicL.filter(isU.patronymic);
        if(!z.isEmpty())
        {
            if(x==-1)
                x = patronymicL.indexOf(z.at(0));
            inU.patronymic = patronymicL.at(x);
        }
        else
            inU.patronymic = "";
    }
}

void FindUser::findDate()
{
    if(isU.birthday != "01:01:1900")
    {
        QStringList z = birthdayL.filter(isU.birthday);
        if(!z.isEmpty())
        {
            if(x==-1)
                x = birthdayL.indexOf(z.at(0));
            inU.birthday = birthdayL.at(x);
        }
        else
            inU.birthday = "";
    }
    x=-1;
    emit returnUser(inU);

}

void FindUser::findBirthday()
{
    if(!isU.birthday.isEmpty())
    {
        QStringList z = birthdayL.filter(isU.birthday);
        if(!z.isEmpty())
        {
            if(x==-1)
                x = birthdayL.indexOf(z.at(0));
            inU.birthday = birthdayL.at(x);
        }
        else
            inU.birthday = "";
    }
}

void FindUser::fLastName()
{
    id_list.clear();
    if(!isU.last_name.isEmpty())
        lnlbuf = last_nameL.filter(isU.last_name);
    if(!lnlbuf.isEmpty())
        for(int i=0;i<lnlbuf.size();i++)
            id_list << id.at(last_nameL.indexOf(lnlbuf.at(i)));
}

void FindUser::fName()
{
    if(!isU.name.isEmpty())
        nlbuf = nameL.filter(isU.name);
    if(!nlbuf.isEmpty())
        for(int i=0;i<nlbuf.size();i++)
            id_list << id.at(nameL.indexOf(nlbuf.at(i)));
}

void FindUser::fPatronymic()
{
    if(!isU.patronymic.isEmpty())
        plbuf = patronymicL.filter(isU.patronymic);
    if(!plbuf.isEmpty())
        for(int i=0;i<plbuf.size();i++)
            id_list << id.at(patronymicL.indexOf(plbuf.at(i)));}

void FindUser::fBirthday()
{
    if(isU.birthday != "01:01:1900")
        blbuf = birthdayL.filter(isU.birthday);
    if(!blbuf.isEmpty())
        for(int i=0;i<blbuf.size();i++)
            id_list << id.at(birthdayL.indexOf(blbuf.at(i)));
}

void FindUser::foreachID()
{
//    for(int i=0; i<id_last_name.size();i++)
//    {
//        id_name.indexOf(id_last_name.at(i))
//    }

}

void FindUser::findUser(FindUser::User u)
{
    refresh();
    isU = u;
    readDB();

    if(!last_nameL.isEmpty())
    {
        findLastName();
        fLastName();
    }
    if(!nameL.isEmpty())
    {
        findName();
        fName();
    }
    if(!patronymicL.isEmpty())
    {
        findPatronymic();
        fPatronymic();
    }
    if(!birthdayL.isEmpty())
    {
        findBirthday();
        fBirthday();
    }
    emit refreshInfo();

}

void FindUser::refresh()
{
    last_nameL.clear();
    lnlbuf.clear();
    nameL.clear();
    nlbuf.clear();
    patronymicL.clear();
    plbuf.clear();
    birthdayL.clear();
    blbuf.clear();
    id.clear();

}
