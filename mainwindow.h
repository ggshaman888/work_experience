#ifndef MAINWINDOW_H
#define MAINWINDOW_H
// http://zakonguru.com/trudovoe/kodeks/stazh/raschet.html
// http://lawyer-consult.ru/administrative/oformlenie-dokumentov/trudovaya-knizhka/zapolnenie/stazh.html
// http://www.consultant.ru/law/ref/calendar/proizvodstvennye/
#include <QMainWindow>
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
#include <QSqlDatabase>
#include "connectiondb.h"
#include <QDialog>
#include <addrengedialog.h>
#include <finduser.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWidget *wgt;
    QLabel *name_program;
    QLabel *last_name_label;
    QLineEdit *last_name;
    QLabel *name_label;
    QLineEdit *name;
    QLabel *patronymic_label;
    QLineEdit *patronymic;
    QLabel *birthday_label;
    QDateEdit *birthday;
    QLabel *range_label;
    QTableWidget *range;
    QLabel *total_label;
    QLabel *total;
    QLineEdit *comment;
    QLabel *comment_label;
    QPushButton *addRange;
    QPushButton *saveCard;
    QPushButton *rmRange;
    QBoxLayout *vertical;
    QBoxLayout *gor2;
    ConnectionDB *c;
    QSqlDatabase db;

    FindUser *user;

    QDialog *addRangeDialog;
    QString test;
    void setTableRange();
    bool infoUser();

    AddRangeDialog *rDialog;


    void createMenus();

    void createActions();
    void createMainWindow();

    QAction *openUsers;
    QAction *openAbout;
    QAction *addUser;

    int id_user = -1;
    void addLink(int);
    void fillWindow();
    int isRangeId();
    void workExperience();
    qint64 difference(QString, QString, QString);
    void updateExperience(qint64);
    QString experienceDays(qint64);
    bool noInDb();

    void replaseStr(QString);

signals:
    void signalAddRange(QDate, QDate, QString, QString);
    void isUser(FindUser::User);

private slots:

    void slotAdd();
    void slotAddRange(AddRangeDialog::RangeDate);
    void slotAddUser();
    void refreshTable();
    void userList();
    void rmStringRange();
    void searchUser();
    void about();
    void userAddAction();
    void ignoreString();

public slots:
    void idUser(int);
    void returnUser(FindUser::User);
};

#endif // MAINWINDOW_H
