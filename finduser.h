#ifndef FINDUSER_H
#define FINDUSER_H

#include <QObject>

class FindUser : public QObject
{
    Q_OBJECT
public:
    explicit FindUser(QObject *parent = 0);

    class User
    {
    public:
        QString last_name;
        QString name;
        QString patronymic;
        QString birthday;
        int id;
    };
    User inU;

    QList<int> id_last_name;
    QList<int> id_name;
    QList<int> id_patronymic;
    QList<int> id_birthday;


    QList<int> id_list;
    void id_list_update();

private:
    void readDB();

    void findLastName();
    void findName();
    void findPatronymic();
    void findDate();
    void findBirthday();

    void fLastName();
    void fName();
    void fPatronymic();
    void fBirthday();

    void foreachID();

    User isU;
    int x = -1;

    QStringList last_nameL, lnlbuf;
    QStringList nameL, nlbuf;
    QStringList patronymicL, plbuf;
    QStringList birthdayL, blbuf;
    QList<int> id;

signals:
    void returnUser(FindUser::User);
    void refreshInfo();

public slots:
    void findUser(FindUser::User);
    void refresh();
};

#endif // FINDUSER_H
