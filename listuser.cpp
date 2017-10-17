#include "listuser.h"
#include "connectiondb.h"
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QtSql>
#include <QTableWidgetItem>
#include <finduser.h>

ListUser::ListUser(QWidget* parent) :
    QDialog( parent )
{

    fuser = new FindUser(this);
    connect(this, SIGNAL(isUser(FindUser::User)), fuser, SLOT(findUser(FindUser::User)));
    connect(fuser, SIGNAL(refreshInfo()), this, SLOT(refreshSearch()));

    list_user_label = new QLabel("Список пользователей");

    loadSearchWidget();

    user_table = new QTableWidget();
    user_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(user_table, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(userIdSlot()));
    connect(user_table, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(deleteLater()));

    setTableUser();

    select = new QPushButton("Выбрать");
    connect(select, SIGNAL(pressed()),
            this, SLOT(userIdSlot()));
    connect(select, SIGNAL(clicked()),
            this, SLOT(deleteLater()));

    cancel = new QPushButton("Отмена");
    connect(cancel, SIGNAL(clicked()),
            this, SLOT(deleteLater()));

    rm = new QPushButton("Удалить");
    connect(rm, SIGNAL(clicked(bool)),
            this, SLOT(rmString()));

    gor = new QBoxLayout(QBoxLayout::LeftToRight);
    vert = new QBoxLayout(QBoxLayout::TopToBottom);

    gor->addWidget(select);
    gor->addWidget(cancel);
    gor->addWidget(rm);

    vert->addLayout(v_max);
    vert->addWidget(list_user_label);
    vert->addWidget(user_table);
    vert->addLayout(gor);

    setLayout(vert);
    setWindowTitle("Список пользователей");
    resize(700, 400);
    refreshTable();
}

void ListUser::refreshTable()
{
    int n = user_table->rowCount();
    for( int i = 0; i < n; i++ ) user_table->removeRow( 0 );

    QSqlQuery query(QSqlDatabase::database("dbsqlite"));
    qDebug() << query.exec("SELECT * FROM user;");

    while (query.next())
    {
        if(trueId(query.value(0).toInt()))
        {
            user_table->insertRow(0);
            user_table->setItem(0, 0, new QTableWidgetItem(query.value(0).toString()));
            user_table->setItem(0, 1, new QTableWidgetItem(query.value(1).toString()));
            user_table->setItem(0, 2, new QTableWidgetItem(query.value(2).toString()));
            user_table->setItem(0, 3, new QTableWidgetItem(query.value(3).toString()));
            user_table->setItem(0, 4, new QTableWidgetItem(query.value(4).toString()));
            user_table->setItem(0, 5, new QTableWidgetItem(query.value(6).toString()));
            user_table->setItem(0, 6, new QTableWidgetItem(query.value(5).toString()));
            user_table->setRowHeight(0, 20);
        }
    }
}

void ListUser::setTableUser()
{
    user_table->setColumnCount(7);
    user_table->setColumnWidth(0,100);
    user_table->setColumnWidth(6,200);
    user_table->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("ID")));
    user_table->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Фамилия")));
    user_table->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Имя")));
    user_table->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Отчество")));
    user_table->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("Дата рождения")));
    user_table->setHorizontalHeaderItem(5, new QTableWidgetItem(tr("Стаж")));
    user_table->setHorizontalHeaderItem(6, new QTableWidgetItem(tr("Комментарий")));
    user_table->setShowGrid(true);
    user_table->setSelectionMode(QAbstractItemView::SingleSelection);
    user_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    user_table->setColumnHidden(0, true);
}

void ListUser::rmString()
{
    QSqlQuery query(QSqlDatabase::database("dbsqlite"));
    QString str = "DELETE FROM user WHERE `id`='%1'";

    str = str.arg(QString::number(isUserId()));

    query.exec(str);

    refreshTable();
}

void ListUser::userIdSlot()
{
    emit userId(isUserId());
}

void ListUser::searchUser()
{
    FindUser::User u;
    u.last_name = last_name->text();
    u.name = name->text();
    u.patronymic = patronymic->text();
    u.birthday = birthday->date().toString("dd:MM:yyyy");
    emit isUser(u);
}

void ListUser::refreshSearch()
{
//    last_name->setText("") = suser;
//    name->setText("");
//    patronymic->setText("");
//    QDate d;
//    d.setDate(1900, 1, 1);
//    birthday->setDate(d);

    refreshTable();

//    QMessageBox *message = new QMessageBox();
//    message->information(this, "Внимание!",  "Несоответствие посковой информации", "ОК");

}

int ListUser::isUserId()
{
    if(!user_table->selectedItems().isEmpty())
        return user_table->selectionModel()->selectedRows().at(0).data().toInt();
    else
    {
        QMessageBox *message = new QMessageBox();
        message->information(this, "Внимание!",  "Выделите ячейку пользователя", "ОК");
    }
    return -1;
}

void ListUser::loadSearchWidget()
{
    what_searh_label = new QLabel("Поиск");
    v_max = new QBoxLayout(QBoxLayout::TopToBottom);
    last_name_label = new QLabel("Фамилия");
    name_label = new QLabel("Имя");
    patronymic_label = new QLabel("Отчество");
    birthday_label = new QLabel("Дата рождения");

    last_name = new QLineEdit();
    connect(last_name, SIGNAL(textEdited(QString)), this, SLOT(searchUser()));
    name = new QLineEdit();
    connect(name, SIGNAL(textEdited(QString)), this, SLOT(searchUser()));
    patronymic = new QLineEdit();
    connect(patronymic, SIGNAL(textEdited(QString)), this, SLOT(searchUser()));
    birthday = new QDateEdit();
    connect(birthday, SIGNAL(editingFinished()), this, SLOT(searchUser()));
    birthday->setDisplayFormat("dd:MM:yyyy");
    QDate d;
    d.setDate(1900, 1, 1);
    birthday->setDate(d);
    v_last_name = new QBoxLayout(QBoxLayout::TopToBottom);

    v_name = new QBoxLayout(QBoxLayout::TopToBottom);
    v_patronymic = new QBoxLayout(QBoxLayout::TopToBottom);
    v_birthday = new QBoxLayout(QBoxLayout::TopToBottom);
    gor_search = new QBoxLayout(QBoxLayout::LeftToRight);

    v_last_name->addWidget(last_name_label);
    v_last_name->addWidget(last_name);
    v_name->addWidget(name_label);
    v_name->addWidget(name);
    v_patronymic->addWidget(patronymic_label);
    v_patronymic->addWidget(patronymic);
    v_birthday->addWidget(birthday_label);
    v_birthday->addWidget(birthday);

    gor_search->addLayout(v_last_name);
    gor_search->addLayout(v_name);
    gor_search->addLayout(v_patronymic);
    gor_search->addLayout(v_birthday);

    v_max->addWidget(what_searh_label);
    v_max->addLayout(gor_search);

}

bool ListUser::trueId(int id)
{
    if(fuser->id_list.isEmpty())
        return true;
    else
    {
        for(int i=0;i<fuser->id_list.size();i++)
            if(id == fuser->id_list.at(i))
                return true;
        return false;
    }
}

