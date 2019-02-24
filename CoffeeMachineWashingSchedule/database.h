#ifndef DATABASE
#define DATABASE


#include <QSqlRelationalTableModel>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QComboBox>

class DataBase
{
    QSqlDatabase db;

    bool createConnection();
    void createTable();

public:
    DataBase(){
        if(!createConnection()){
            exit(-1);
        }
        createTable();
    }
    void clear(QString tblname);
    QSqlQuery select(QString column);
    void insertRow(QString number, QString surname, QString name, QString phone, QString email);
    void fillComboBox(QSqlQuery query, QComboBox *box, QString column);
    void remove();
    void insertIntoStatus(QString date, QString number);
    QSqlQuery selectFromStatus(QString column);
};

#endif // DATABASE

