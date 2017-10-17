#ifndef LISTUSER_H
#define LISTUSER_H

#include <QObject>
#include <QDate>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QList>
#include <QLayout>
#include <QBoxLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QTableView>
#include <QTableWidget>
#include <QDebug>
#include <QMessageBox>
#include <QCheckBox>
#include <QTableWidgetItem>
#include <finduser.h>

class ListUser : public QDialog
{
    Q_OBJECT
public:
    ListUser(QWidget* parent=0);

private:
    QLabel *list_user_label;
    QTableWidget *user_table;
    QPushButton *select;
    QPushButton *cancel;
    QPushButton *rm;
    QBoxLayout *gor;
    QBoxLayout *vert;

    QLabel *what_searh_label;
    QBoxLayout *v_max;
    QLabel *last_name_label;
    QLabel *name_label;
    QLabel *patronymic_label;
    QLabel *birthday_label;
    QLineEdit *last_name;
    QLineEdit *name;
    QLineEdit *patronymic;
    QDateEdit *birthday;
    QBoxLayout *v_last_name;
    QBoxLayout *v_name;
    QBoxLayout *v_patronymic;
    QBoxLayout *v_birthday;
    QBoxLayout *gor_search;

    void refreshTable();
    void setTableUser();
    int isUserId();
    void loadSearchWidget();

    FindUser *fuser;
    bool trueId(int);

signals:
    void userId(int);
    void isUser(FindUser::User);

private slots:
    void rmString();
    void userIdSlot();
    void searchUser();

public slots:
    void refreshSearch();

};

#endif // LISTUSER_H
