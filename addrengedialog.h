#ifndef ADDRENGEDIALOG_H
#define ADDRENGEDIALOG_H

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

class AddRangeDialog : public QDialog
{
    Q_OBJECT

public:
    AddRangeDialog(QWidget* parent=0);
   // ~AddRengeDialog();

    class RangeDate
    {
    public:
        QDate start;
        QDate end;
        bool subtraction;
        QString comment;
    };
    void setForm();

signals:
    void getInput(AddRangeDialog::RangeDate);

private slots:
    void input();
    void refresh();

private:
    QLabel *startLabel;
    QLabel *endLabel;
    QLabel *commentLabel;
    QDateEdit *startDate;
    QDateEdit *endDate;
    QCheckBox *subtraction;
    QLineEdit *comment;
    QPushButton *ok;
    QPushButton *back;
    QBoxLayout *gor;
    QBoxLayout *vert;

    bool inputTrue(RangeDate);
};

#endif // ADDRENGEDIALOG_H
