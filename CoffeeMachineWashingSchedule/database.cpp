#include "database.h"
#include <QSqlRelationalTableModel>
#include <QSqlRecord>


bool DataBase :: createConnection()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("main");
    db.setUserName("elton");
    db.setHostName("epica");
    db.setPassword("password");
    if (!db.open()) {
        qDebug() << "Cannot open database:" << db.lastError();
        return false;
    }
    return true;
}

void DataBase::insertRow(QString number, QString surname,  QString name, QString phone, QString email){
    QSqlQuery query;
    query.prepare("INSERT INTO students (number, surname, name, phone, email)"
    "VALUES(:number, :surname, :name, :phone, :email);");
    query.bindValue(":number", number);
    query.bindValue(":surname", surname);
    query.bindValue(":name", name);
    query.bindValue(":phone", phone);
    query.bindValue(":email", email);
    query.exec();



    /*query.prepare("INSERT INTO status (number, married)"
                " VALUES(2, 'NO');");
    query.exec()*/
}


void DataBase::insertIntoStatus(QString date, QString number){
    QSqlQuery query;
    query.prepare("INSERT INTO status (date, number)"
                  "VALUES(:date, :number);");
    query.bindValue(":date", date);
    query.bindValue(":number", number);
    query.exec();
}

void DataBase::createTable()
{
    QSqlQuery query;
    QString str = "CREATE TABLE students ( "
        "number INTEGER PRIMARY KEY NOT NULL, "
        "surname VARCHAR(15), "
        "name VARCHAR(15), "
        "phone VARCHAR(12), "
        "email VARCHAR(15) "
        ");";

    if (!query.exec(str)) {
        qDebug() << "Unable to create a table";
    }

     str = "CREATE TABLE status ( "
        "date VARCHAR(10) PRIMARY KEY NOT NULL, "
        "number INTEGER"
        ");";

     if (!query.exec(str)) {
         qDebug() << "Unable to create a table";
     }
}

QSqlQuery DataBase:: select(QString column){
    QSqlQuery query;
    if (!query.exec("SELECT " + column + " FROM students;")) {
        qDebug() << "Unable to execute query — exiting";
        exit(-1);
    }
    return query;
}

QSqlQuery DataBase:: selectFromStatus(QString column){
    QSqlQuery query;
    if (!query.exec("SELECT " + column + " FROM status;")) {
        qDebug() << "Unable to execute query — exiting";
        exit(-1);
    }
    return query;
}


void DataBase::fillComboBox(QSqlQuery query, QComboBox *box, QString column){
    QSqlRecord rec = query.record();
    QString strName;
    while (query.next()) {
        strName = query.value(rec.indexOf(column)).toString();
        qDebug() << strName << '\n';
        box->addItem(strName);
    }
}


void DataBase::clear(QString tblname){
    QSqlQuery query;
    query.prepare("DROP TABLE " + tblname);
    query.exec();
}

void DataBase::remove(){
    db.close();
    QSqlDatabase::removeDatabase("students");
}
