#include "mainwindow.h"
#include <QMessageBox>
#include <QColor>
#include <iostream>
#include <QSqlRecord>
#include "formbd.h"


MainWindow::MainWindow(QWidget *parent) : QDialog(parent)
{
    box1 = new QComboBox;
    box1->addItem("--");
    //std::cout << box1->itemText(0).toStdString() << '\n';
    box2 = new QComboBox;
    QStringList lst;
    lst << "BD-11" << "BD-21" << "BD-31" << "BD-41";
    box2->addItems(lst);

    calendar = new NewCalendarWidget;
    //calendar->paintBirthDay();

    button2 = new QPushButton(trUtf8("Даты назначенных дежурств"));
    button3 = new QPushButton(trUtf8("Ок"));
    buttonadd = new QPushButton(trUtf8("Добавить студента"));
    buttonchoose = new  QCheckBox(trUtf8("Назначить дежурство автоматически"));
    buttonmonday = new  QCheckBox(trUtf8("Понедельник"));
    buttontuesday = new  QCheckBox(trUtf8("Вторник"));
    buttonwednesday = new  QCheckBox(trUtf8("Среда"));
    buttonthursday = new  QCheckBox(trUtf8("Четверг"));
    buttonfriday = new  QCheckBox(trUtf8("Пятница"));
    buttonsaturday = new  QCheckBox(trUtf8("Суббота"));
    buttonsunday = new  QCheckBox(trUtf8("Воскресенье"));
    notice = new QTextEdit();

    lbl1 = new QLabel(trUtf8("Выберите студента из списка"));
    pic = new QLabel;
    button1 = new QPushButton(trUtf8("Просмотреть базу студентов"));
    naznachit = new QPushButton(trUtf8("Назначить"));
    if(box1->currentIndex() == 0)
        naznachit->setEnabled(false);
    //notice = new QTextEdit;
    dateEdit = new QDateEdit;
    dateEdit->setDate(calendar->selectedDate());
    dateEdit->setDisplayFormat("dd MMMM yyyy");

    QLabel *dateLabel = new QLabel(tr("Дата:"));
    monthCombo = new QComboBox;
    for (int month = 1; month <= 12; ++month)
     monthCombo->addItem(QDate::longMonthName(month));
    monthCombo->setEnabled(false);

    monthCombo2 = new QComboBox;
    for (int month = 1; month <= 12; ++month)
     monthCombo2->addItem(QDate::longMonthName(month));
    monthCombo2->setEnabled(false);

    dayEdit = new QDateTimeEdit;
    dayEdit->setDisplayFormat("dd");
    dayEdit->setDateRange (QDate(2017, 1, 1), QDate(2020, 1, 1));
    dayEdit->setEnabled(false);

    dayEdit2 = new QDateTimeEdit;
    dayEdit2->setDisplayFormat("dd");
    dayEdit2->setDateRange (QDate(2017, 1, 1), QDate(2020, 1, 1));
    dayEdit2->setEnabled(false);

    QVBoxLayout *right = new QVBoxLayout;
    //right->addLayout(layout);
    right->addWidget(buttonchoose);

    QVBoxLayout *rightbottom = new QVBoxLayout;
    rightbottom->addWidget(monthCombo);
    rightbottom->addWidget(dayEdit);
    rightbottom->addWidget(monthCombo2);
    rightbottom->addWidget(dayEdit2);
    rightbottom->addWidget(buttonmonday);
    rightbottom->addWidget(buttontuesday);
    rightbottom->addWidget(buttonwednesday);
    rightbottom->addWidget(buttonthursday);
    rightbottom->addWidget(buttonfriday);
    rightbottom->addWidget(buttonsaturday);
    rightbottom->addWidget(buttonsunday);
    rightbottom->addWidget(button3);

    buttonmonday ->setEnabled(false);
    buttontuesday ->setEnabled(false);
    buttonwednesday ->setEnabled(false);
    buttonthursday ->setEnabled(false);
    buttonfriday ->setEnabled(false);
    buttonsaturday ->setEnabled(false);
    buttonsunday ->setEnabled(false);
    button3 ->setEnabled(false);
    right->addLayout(rightbottom);


    QPixmap myPixmap( "technosphera_logo.jpg" );
    pic->setPixmap( myPixmap );

    right->addWidget(pic);

    QVBoxLayout *left = new QVBoxLayout;
    left->addWidget(lbl1);
    left->addWidget(box2);
    left->addWidget(box1);
    left->addWidget(naznachit);
    left->addWidget(button2);
    left->addWidget(button1);
    left->addWidget(buttonadd);
    left->addWidget(dateEdit);
    left->addWidget(box1);
    left->addWidget(box2);
    left->addWidget(calendar);

    QHBoxLayout *main = new QHBoxLayout;
    main->addLayout(right);
    main->addLayout(left);


    connect(naznachit, SIGNAL(clicked(bool)), this, SLOT(paintCalendar()));

    connect(box1, SIGNAL(currentIndexChanged(int)), this, SLOT(comboboxIsNotDefault(int)));

    connect(buttonadd,SIGNAL(clicked()),this,SLOT(button3Clicked()));

    connect(calendar, SIGNAL(clicked(QDate)), dateEdit, SLOT(setDate(QDate)));

    connect(calendar, SIGNAL(clicked(QDate)), this, SLOT(updateComboBox(QDate)));

    connect(buttonchoose,SIGNAL(stateChanged(int)),this,SLOT(rightbottomlayotEnabled()));

    setLayout(main);
}

void MainWindow :: rightbottomlayotEnabled()
{
    if (buttonchoose->isChecked())
    {
    monthCombo->setEnabled(true);
    dayEdit->setEnabled(true);
    monthCombo2->setEnabled(true);
    dayEdit2->setEnabled(true);
    buttonmonday ->setEnabled(true);
    buttontuesday ->setEnabled(true);
    buttonwednesday ->setEnabled(true);
    buttonthursday ->setEnabled(true);
    buttonfriday ->setEnabled(true);
    buttonsaturday ->setEnabled(true);
    buttonsunday ->setEnabled(true);
    button3 ->setEnabled(true);
    }
}

void MainWindow :: button3Clicked()
{
    FormBd *w = new FormBd() ;
    w->setAttribute(Qt::WA_DeleteOnClose, true);
    w->show();

    connect(w, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

    w->setWindowTitle(trUtf8("Добавить студента"));
   //w->exec();
}

void MainWindow::slotUpdateModels()
{
   //нужно обновить таблицу с бд и combobox
}

void MainWindow :: comboboxIsNotDefault(int Index){
   // std::cout << Index << std::endl;
    naznachit->setEnabled(Index);
}


void MainWindow :: paintCalendar(){
    QDate date = dateEdit->date();
    calendar->paintBirthDay(date);
    QString str = box1->currentText();
    db.insertIntoStatus(date.toString("dd.MM.yyyy"), str);
}

void MainWindow :: updateComboBox(QDate date){
    QString datestr = date.toString("dd.MM.yyyy");
    QSqlQuery query = db.selectFromStatus("*");
    QSqlRecord rec = query.record();
    QString str;
    while (query.next()) {
        str = query.value(rec.indexOf("date")).toString();
        if(str == datestr){
            str = query.value(rec.indexOf("number")).toString();
            //box1->setCurrentText(str);
            return;
        }

    }
    //box1->setCurrentIndex(0);
}
