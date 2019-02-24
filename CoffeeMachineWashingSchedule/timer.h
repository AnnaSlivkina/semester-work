#ifndef TIMER_H
#define TIMER_H


#include <QDialog>
#include <QMessageBox>

#include <database.h>

class Timer : public QDialog
{
    Q_OBJECT

public:
    Timer(QDialog *parent = 0);

public:
    void FindCurrentDate();

   };

#endif // TIMER_H
