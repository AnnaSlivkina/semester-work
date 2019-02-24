#ifndef NEWCALENDARWIDGET
#define NEWCALENDARWIDGET

#include <QCalendarWidget>
#include <QColor>
#include <QDate>
#include <QPen>
#include <QBrush>
#include "database.h"

class NewCalendarWidget : public QCalendarWidget
{
Q_OBJECT

Q_PROPERTY(QColor color READ getColor WRITE setColor)

public:
   NewCalendarWidget(QWidget *parent = 0);
   ~NewCalendarWidget();

   void setColor(QColor& color);
   QColor getColor();

   void paintBirthDay(QDate date);

protected:
   virtual void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const;

private:
   QDate m_currentDate;
   QPen m_outlinePen;
   QBrush m_transparentBrush;

   DataBase db;

};

#endif // NEWCALENDARWIDGET

