#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QLabel>
#include <QList>
#include <QLayout>
#include <QBoxLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QTableView>
#include <QTableWidget>
#include <QDate>
#include "connectiondb.h"
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QtSql>
#include <QMessageBox>
#include <QRadioButton>
#include <QCheckBox>
#include <addrengedialog.h>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <listuser.h>
#include <finduser.h>
#include <calculation.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createMainWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTableRange()
{
    range->setColumnCount(5);
    range->setColumnWidth(0,20);
    range->setColumnWidth(1,20);
    range->setColumnWidth(4,260);
    range->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("х")));
    range->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("+-")));
    range->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("DateStart")));
    range->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("DateEnd")));
    range->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("Comment")));
    range->setShowGrid(true);
    range->setSelectionMode(QAbstractItemView::SingleSelection);
    range->setSelectionBehavior(QAbstractItemView::SelectRows);
   // range->setColumnHidden(0, true);
}

bool MainWindow::infoUser()
{
    if(last_name->text()!="")
        if(name->text()!="")
            if(patronymic->text()!="")
                if(birthday->date().year()>1900)
                    if(noInDb())
                        return true;
    return false;
}

void MainWindow::refreshTable()
{
     int n = range->rowCount();
     for( int i = 0; i < n; i++ ) range->removeRow( 0 );

     QSqlQuery query(QSqlDatabase::database("dbsqlite"));
     query.prepare("SELECT * FROM range WHERE id IN "
    "(SELECT id_range FROM connection WHERE id_user = :id);");
     query.bindValue(":id", QString::number(id_user));

     query.exec();
     query.lastError();
     while (query.next())
     {
         QString sub;
         if(query.value(3).toString()=="1")
             sub="-";
         else sub="+";

          range->insertRow(0);
          range->setItem(0, 0, new QTableWidgetItem(QChar(9745))); // симол галочка
          range->setItem(0, 2, new QTableWidgetItem(query.value(1).toString()));
          range->setItem(0, 3, new QTableWidgetItem(query.value(2).toString()));
          range->setItem(0, 1, new QTableWidgetItem(sub));
          range->setItem(0, 4, new QTableWidgetItem(query.value(4).toString()));
          range->setRowHeight(0, 20);
     }
     range->model()->sort(0, Qt::DescendingOrder);
     workExperience();

}

void MainWindow::userList()
{
    ListUser * user_list = new ListUser(this);
    connect(user_list, SIGNAL(userId(int)), this, SLOT(idUser(int)));
    user_list->show();
}

void MainWindow::idUser(int id)
{
    id_user = id;
    fillWindow();
}

void MainWindow::returnUser(FindUser::User u)
{
    last_name->setText(u.last_name);
    name->setText(u.name);
    patronymic->setText(u.patronymic);
    birthday->setDate(QDate::fromString(u.birthday, "dd.MM.yyyy"));
}

void MainWindow::slotAdd()
{
    if(id_user>-1)
    {
        rDialog = new AddRangeDialog(this);

        connect(rDialog, SIGNAL(getInput(AddRangeDialog::RangeDate)),
                this, SLOT(slotAddRange(AddRangeDialog::RangeDate)));
        rDialog->exec();
    }
    else
    {
        QMessageBox *message = new QMessageBox();
        message->information(this, "Внимание!",  "Пользователь не выбран. "
        "Выберите пользователя, или сохраните текущего", "ОК");
    }

}

void MainWindow::slotAddRange(AddRangeDialog::RangeDate range_date)
{
    QSqlQuery query(QSqlDatabase::database("dbsqlite"));
    QString str = "INSERT INTO range (date_start, date_end, subtraction, comments) "
                  "VALUES ('%1', '%2', '%3', '%4')";

    str = str.arg(range_date.start.toString("dd.MM.yyyy"))
            .arg(range_date.end.toString("dd.MM.yyyy"))
            .arg(range_date.subtraction)
            .arg(range_date.comment);

    query.exec(str);

    addLink(query.lastInsertId().toInt());

    refreshTable();
}

void MainWindow::slotAddUser()
{
    QSqlQuery query(QSqlDatabase::database("dbsqlite"));
    if(infoUser())
    {
        if(saveCard->text()=="Добавить нового работника")
        {
            QString str = "INSERT INTO user (last_name, name, patronymic, birthday, comments) "
                          "VALUES ('%1', '%2', '%3', '%4', '%5')";

            str = str.arg(last_name->text())
                    .arg(name->text())
                    .arg(patronymic->text())
                    .arg(birthday->date().toString("dd.MM.yyyy"))
                    .arg(comment->text());

            query.exec(str);
            qDebug()<< str << query.lastError();
            id_user = query.lastInsertId().toInt();
            saveCard->setText("Изменить данные работника");
        }
        else
        {
            QString str = "UPDATE user SET last_name = '%1', "
                          "name = '%2', patronymic = '%3', "
                          "birthday = '%4', comments = '%5' "
                          "WHERE id = '%6";

            str = str.arg(last_name->text())
                    .arg(name->text())
                    .arg(patronymic->text())
                    .arg(birthday->date().toString("dd.MM.yyyy"))
                    .arg(comment->text())
                    .arg(QString::number(id_user));

            query.exec(str);
            qDebug()<< str << query.lastError();
            id_user = query.lastInsertId().toInt();

        }
    }
    else
    {
        QMessageBox *message = new QMessageBox();
        message->information(this, "Внимание!",
        "Такой работник уже есть в базе, либо форма не заполненна", "ОК");
    }
    refreshTable();
}



void MainWindow::createMenus()
{
    createActions();

    QMenu * mnFile = new QMenu("&Файл");
    QMenu * mnHelp = new QMenu("&Помощь");

    mnFile->addAction(openUsers);
    mnFile->addAction(addUser);
    mnHelp->addAction(openAbout);
    ui->menuBar->addMenu(mnFile);
    ui->menuBar->addMenu(mnHelp);


}

void MainWindow::createActions()
{
    openUsers = new QAction(tr("&Работники"), this);
    connect(openUsers, SIGNAL(triggered()), this, SLOT(userList()));
    addUser = new QAction(tr("&Добавить работника"), this);
    connect(addUser, SIGNAL(triggered()), this, SLOT(userAddAction()));
    openAbout = new QAction(tr("&О нас"), this);
    connect(openAbout, SIGNAL(triggered()), this, SLOT(about()));

}

void MainWindow::createMainWindow()
{
    createMenus();
    QString p = QApplication::applicationDirPath();
    p.append("/icon.png");
    setWindowIcon(QIcon(p));

    c = new ConnectionDB;
    p = QApplication::applicationDirPath();
    p.append("/base.db");
    qDebug() << p;
    c->createConnectionMYSQLite("/mnt/tmpfs/base.db");
    db = QSqlDatabase::addDatabase("QSQLITE");
    if (!db.open())
    {
        QMessageBox::warning( 0 , "Ошибка!", db.lastError().databaseText());
    }

    user = new FindUser();
    connect(user, SIGNAL(returnUser(FindUser::User)), this, SLOT(returnUser(FindUser::User)));
    connect(this, SIGNAL(isUser(FindUser::User)), user, SLOT(findUser(FindUser::User)));

  //  name_program = new QLabel("Программа подсчёта стажа v.1.1");
    last_name_label = new QLabel("Фамилия");
    last_name = new QLineEdit();
    name_label = new QLabel("Имя");
    name = new QLineEdit();
    patronymic_label = new QLabel("Отчество");
    patronymic = new QLineEdit();
    birthday_label = new QLabel("Дата рождения");
    birthday = new QDateEdit();
    birthday->setDisplayFormat("dd.MM.yyyy");
    comment_label = new QLabel("Комментарий");
    comment = new QLineEdit();
    QDate d;
    d.setDate(1900, 1, 1);
    birthday->setDate(d);
    range_label = new QLabel("Диапазоны работы");
    range = new QTableWidget();
    connect(range, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(ignoreString()));
    range->setEditTriggers(QAbstractItemView::NoEditTriggers);
   // range->resize(300, 200);
    setTableRange();
    total_label = new QLabel("Стаж итого:");
    total = new QLabel("00:00:0000");

    addRange = new QPushButton("Добавить диапазон");
    connect(addRange, SIGNAL(clicked()), this, SLOT(slotAdd()));
    rmRange = new QPushButton("Удалить диапазон");
    connect(rmRange, SIGNAL(clicked()), this, SLOT(rmStringRange()));
    gor2 = new QBoxLayout(QBoxLayout::LeftToRight);
    gor2->addWidget(addRange);
    gor2->addWidget(rmRange);

    saveCard = new QPushButton("Добавить нового работника");
    connect(saveCard, SIGNAL(clicked()), this, SLOT(slotAddUser()));
    vertical = new QBoxLayout(QBoxLayout::TopToBottom);

    //vertical->addWidget(name_program);
    vertical->addWidget(last_name_label);
    vertical->addWidget(last_name);
    vertical->addWidget(name_label);
    vertical->addWidget(name);
    vertical->addWidget(patronymic_label);
    vertical->addWidget(patronymic);
    vertical->addWidget(birthday_label);
    vertical->addWidget(birthday);
    vertical->addWidget(comment_label);
    vertical->addWidget(comment);
    vertical->addWidget(range_label);
    vertical->addWidget(range);
    vertical->addWidget(total_label);
    vertical->addWidget(total);
    vertical->addLayout(gor2);
    vertical->addWidget(saveCard);

    ui->centralWidget->setLayout(vertical);
    setWindowTitle("Расчёт стажа");
}

void MainWindow::addLink(int id)
{
    QSqlQuery query(QSqlDatabase::database("dbsqlite"));
    QString str = "INSERT INTO connection (id_user, id_range) "
                  "VALUES ('%1', '%2')";

    str = str.arg(id_user).arg(id);

    query.exec(str);
}

void MainWindow::fillWindow()
{
    QSqlQuery query(QSqlDatabase::database("dbsqlite"));
    QString str = "SELECT * FROM user WHERE id = '%1';";
    str = str.arg(QString::number(id_user));
    qDebug() << query.exec(str) << str;
    qDebug() << query.lastError();

    while (query.next())
    {
         last_name->setText(query.value(1).toString());
         name->setText(query.value(2).toString());
         patronymic->setText(query.value(3).toString());
         birthday->setDate(QDate::fromString(query.value(4).toString(), "dd.MM.yyyy"));
         comment->setText(query.value(5).toString());
    }
    saveCard->setText("Изменить данные работника");
    refreshTable();
}

int MainWindow::isRangeId()
{
    if(!range->selectedItems().isEmpty())
        return range->selectionModel()->selectedRows().at(0).data().toInt();
    else
    {
        QMessageBox *message = new QMessageBox();
        message->information(this, "Внимание!",  "Выделите ячейку диапазона", "ОК");
        return -1;
    }
}

void MainWindow::workExperience()
{
    Calculation *calc = new Calculation();
    for(int i=0; i<range->rowCount();++i)
        if(range->item(i, 0)->text()==QChar(9745))
            calc->setValue(range->item(i, 2)->text(), range->item(i, 3)->text(), range->item(i, 1)->text());

    QString experience = calc->calculate();
    total->setText(experience);

    QSqlQuery query(QSqlDatabase::database("dbsqlite"));
    QString str = "UPDATE user SET experience = '%1' WHERE id = '%2';";
    str = str.arg(experience).arg(id_user);
}

qint64 MainWindow::difference(QString start, QString end, QString p)
{
    QDate x, y;
    x = QDate::fromString(start, "dd.MM.yyyy");
    y = QDate::fromString(end, "dd.MM.yyyy");
    qint64 z = x.daysTo(y);
    if(p=="-")
        z = z*-1;
    return z;
}


bool MainWindow::noInDb()
{
    QSqlQuery query(QSqlDatabase::database("dbsqlite"));
    QString str = "SELECT * FROM user WHERE last_name = '%1' AND "
                  "name = '%2' AND patronymic = '%3' AND "
                  "birthday = '%4';";

    str = str.arg(last_name->text()).arg(name->text())
            .arg(patronymic->text())
            .arg(birthday->date().toString("dd.MM.yyyy"));

    qDebug() << query.exec(str) << str;
    qDebug() << query.lastError();

    if(!query.next())
        return true;
    return false;
}

void MainWindow::rmStringRange()
{
    int x = isRangeId();
    if(x>-1)
    {
        QSqlQuery query(QSqlDatabase::database("dbsqlite"));
        QString str = "DELETE FROM range WHERE `id`='%1'";

        str = str.arg(QString::number(x));

        query.exec(str);

        refreshTable();
    }
}

void MainWindow::searchUser()
{
    FindUser::User u;
    u.last_name = last_name->text();
    u.name = name->text();
    u.patronymic = patronymic->text();
    u.birthday = birthday->date().toString("dd.MM.yyyy");
    emit isUser(u);
}

void MainWindow::about()
{
    QMessageBox *message = new QMessageBox();
    message->information(this, "О программе",  "\"Опыт работы\" версия 1.1. "
   " Разработчик - Красовский Александр Вячеславович. E-mail для связи - krasovsky.av@yandex.ru "
   " Официальный сайт - www.sitefresh.net", "ОК");

}

void MainWindow::userAddAction()
{
    id_user = -1;
    last_name->setText("");
    name->setText("");
    patronymic->setText("");
    QDate d;
    d.setDate(1900, 1, 1);
    birthday->setDate(d);
    comment->setText("");

    saveCard->setText("Добавить нового работника");
}

void MainWindow::ignoreString()
{
    if(!range->selectedItems().isEmpty())
        replaseStr(range->selectionModel()->selectedRows().at(0).data().toString());
    else
    {
        QMessageBox *message = new QMessageBox();
        message->information(this, "Внимание!",  "Что то не так", "ОК");
    }
}

void MainWindow::replaseStr(QString str)
{
    int x = range->currentRow();
    qDebug() << x;
    if(str==QChar(9745))
        range->setItem(x, 0, new QTableWidgetItem(QChar(10720))); // симол галочка
    else
        range->setItem(x, 0, new QTableWidgetItem(QChar(9745))); // симол галочка
    workExperience();
}
