#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QMainWindow>
#include <QComboBox>
#include <QCalendarWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QWidget>
#include <QDateEdit>
#include "newcalendarwidget.h"
#include "database.h"
#include <QCheckBox>



class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent);

private:

    DataBase db;

    QComboBox *box2;
    NewCalendarWidget *calendar;
    QLabel *lbl1;
    QLabel *lbl2;
    QLabel *pic;
    QLineEdit *line;


    QPushButton *button3;
    QPushButton *button4;
    QTextEdit *notice;
    QDateEdit *dateEdit;

    QCheckBox *buttonchoose;
    QCheckBox *buttonmonday;
    QCheckBox *buttontuesday;
    QCheckBox *buttonwednesday;
    QCheckBox *buttonthursday;
    QCheckBox *buttonfriday;
    QCheckBox *buttonsaturday;
    QCheckBox *buttonsunday;

   QPushButton *buttonadd;

   QComboBox *monthCombo;
   QDateTimeEdit *dayEdit;
   QComboBox *monthCombo2;
   QDateTimeEdit *dayEdit2;


private slots:
    void rightbottomlayotEnabled();
    void button3Clicked();
    void comboboxIsNotDefault(int);
    void paintCalendar();
    void updateComboBox(QDate date);
    void slotUpdateModels();
    //void insertDateInNotice(QDate date);
//    void TextChanged(QString str);

public:
    int createDB();
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *naznachit;
    QComboBox *box1;

};




#endif // MAINWINDOW_H
