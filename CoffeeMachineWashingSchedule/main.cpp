#include <QApplication>
#include <QSqlQuery>
#include <QTableView>
#include "mainwindow.h"
#include "database.h"
#include <timer.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //____________________________________________________________________


    //____________________________________________________________________
    //создаём ебучую базу данных
    DataBase db = DataBase();

    //добавляем ебучую строку вот таким блять методом (ваш кэп)
    db.insertRow("1", "Матвеева", "Виктория", "000", "gagag");
    db.insertRow("2", "Сливкина", "Анна", "000000", "2drgerhser");


    //белиберда, которую надо сделать, чтобы получить окошко с таблицей (view)
    QSqlRelationalTableModel model;
    model.setTable("students");
    //model.setRelation(0, QSqlRelation("status", "date", "number"));
    model.select();
    model.setEditStrategy(QSqlRelationalTableModel::OnFieldChange);
    QTableView view;
    view.setModel(&model);
    //белиберда заканчивается (в отдельный метод не получилось внести, ну да пох)

    //db.clear("status");
    QSqlRelationalTableModel model1;
    model1.setTable("status");
    //model.setRelation(0, QSqlRelation("status", "date", "number"));
    model1.select();
    model1.setEditStrategy(QSqlRelationalTableModel::OnFieldChange);
    QTableView view1;
    view1.setModel(&model1);

    MainWindow *window = new MainWindow(0);

    //вот ето конкретно тож самое что и SELECT * FROM students (imya ebuchey DB)
    QSqlQuery query = db.select("*");

    //ну и заполняем успевший нас запарить комбобоксмастер :D
    db.fillComboBox(query, window->box1, "surname");

    QObject::connect(window->button1, SIGNAL(clicked(bool)), &view, SLOT(show()));
    QObject::connect(window->button2, SIGNAL(clicked(bool)), &view1, SLOT(show()));
    window->show();

    Timer mytimer;
    mytimer.FindCurrentDate();

    return a.exec();
}

