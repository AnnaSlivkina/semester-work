#ifndef FORMBD_H
#define FORMBD_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>

#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QMessageBox>

#include <database.h>

class FormBd : public QDialog
{
    Q_OBJECT

public:
    FormBd(QDialog *parent = 0);

signals:
    void signalReady();

private slots:
    void on_buttonok_accepted();

private:
    QLabel *name;
    QLabel *surname;
    QLabel *group;
    QLabel *email;
    QLabel *phone;
    QComboBox *boxgroup;
    QPushButton *buttonok;
    QLineEdit *linename;
    QLineEdit *linesurname;
    QLineEdit *lineemail;
    QLineEdit *linephone;

    QSqlTableModel              *model;
    QDataWidgetMapper           *mapper;

private:
    void setupModel();
    void accept();

   };


#endif // FORMBD_H
