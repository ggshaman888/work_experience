#include "addrengedialog.h"

AddRangeDialog::AddRangeDialog(QWidget* parent) :
    QDialog( parent,  Qt::WindowTitleHint | Qt::WindowSystemMenuHint )
{
    startLabel = new QLabel("Начало:");
    endLabel = new QLabel("Конец:");
    commentLabel = new QLabel("Комментарий:");
    startDate = new QDateEdit();
    startDate->setDisplayFormat("dd:MM:yyyy");
    endDate = new QDateEdit();
    endDate->setDisplayFormat("dd:MM:yyyy");
    subtraction = new QCheckBox();
    subtraction->setText("Вычесть диапазон?");
    comment = new QLineEdit();
    ok = new QPushButton();
    ok->setText("Добавить");
    connect(ok, SIGNAL(clicked()), this, SLOT(input()));
    connect(ok, SIGNAL(clicked()), this, SLOT(refresh()));

    back = new QPushButton();
    back->setText("Отмена");
    connect(back, SIGNAL(clicked()), this, SLOT(deleteLater()));

    gor = new QBoxLayout(QBoxLayout::LeftToRight);
    gor->addWidget(ok);
    gor->addWidget(back);

    vert = new QBoxLayout(QBoxLayout::TopToBottom);
    vert->addWidget(startLabel);
    vert->addWidget(startDate);
    vert->addWidget(endLabel);
    vert->addWidget(endDate);
    vert->addWidget(subtraction);
    vert->addWidget(commentLabel);
    vert->addWidget(comment);
    vert->addLayout(gor);
    setLayout(vert);
    setWindowTitle("Добавление диапазона");
}

void AddRangeDialog::setForm()
{

}

void AddRangeDialog::input()
{
    RangeDate range;
    range.start = startDate->date();
    range.end = endDate->date();
    range.subtraction = subtraction->isChecked();
    range.comment = comment->text();
    if (inputTrue(range))
        emit getInput(range);
    else
    {
        QMessageBox *message = new QMessageBox();
        message->information(this, "Внимание!",
        "Данные не добавленны. Не корректно заполненны поля.", "ОК");
    }
}

void AddRangeDialog::refresh()
{
    QDate x;
    x.setDate(1900,01,01);
    startDate->setDate(x);
    endDate->setDate(x);
    comment->setText("");
    subtraction->setChecked(false);
    startDate->setFocus();
}

bool AddRangeDialog::inputTrue(AddRangeDialog::RangeDate range)
{
    if(range.start==range.end)
        return false;

    if(range.start.year()>1900)
        if(range.start.daysTo(range.end)>0)
            return true;

    return false;
}
