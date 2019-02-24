#include "timer.h"
#include <QDate>


Timer::Timer(QDialog *parent) :QDialog(parent)
{

        FindCurrentDate();

}

void Timer::FindCurrentDate()
{
    QDate dateToday = QDate::currentDate();
    QString str;
    QString str1;
    str = dateToday.toString("dd.MM.yyyy");

    QSqlQuery query;

    str1 = "SELECT * FROM status WHERE date = ";
    str1 = str1 + "'" + str + "'";

    //query.exec ("SELECT * FROM status WHERE date = " + "'" + str + "'");
    query.exec(str1);

    if(!query.isActive())
     QMessageBox::warning(this, ("Database Error"), query.lastError().text());

    //Если нашел такую дату -> Найти surname студента -> Узнает мэйл студента -> Послать ему уведомление

    QString numberstatus;
    while (query.next()) {
     numberstatus = query.value(0).toString();
     //qDebug()<<"Huinya"<<numberstatus<<'\n';

    QString strsurname;
    QString strsurname1;
    QSqlQuery querysurname;

    strsurname = "SELECT email FROM students WHERE surname = ";
    strsurname1 = strsurname + "'" + numberstatus + "'";

    querysurname.exec(strsurname);

    if(!querysurname.isActive())
     QMessageBox::warning(this, ("Database Error"), querysurname.lastError().text());


    }
    //Если не нашел -> Уведомление админу о том, что надо назначить



    //connect(buttonok,SIGNAL(clicked()),this,SLOT(on_buttonok_accepted()));
}







