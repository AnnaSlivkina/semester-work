#include <QPainter>
#include "newcalendarwidget.h"
#include <QTextCharFormat>
#include <QSqlRecord>
#include <QDebug>
#include <iostream>

NewCalendarWidget::NewCalendarWidget(QWidget *parent)
   : QCalendarWidget(parent)
{
   m_currentDate = QDate::currentDate();
   m_outlinePen.setColor(Qt::red);
   m_transparentBrush.setColor(Qt::transparent);

   QSqlQuery query = db.selectFromStatus("date");
   QSqlRecord rec = query.record();
   QString str;
   QDate date;
   while(query.next()){
       std::cout << "Cicleeeeeeeee\n";
       str = query.value(rec.indexOf("date")).toString();
       date = QDate::fromString(str, "dd.MM.yyyy");
       //qDebug() << date.toString("dd.MM.yyyy") << '\n';
       qDebug() << date << '\n';
       this->paintBirthDay(date);
   }
}

NewCalendarWidget::~NewCalendarWidget()
{

}

void NewCalendarWidget::setColor(QColor& color)
{
   m_outlinePen.setColor(color);
}

QColor NewCalendarWidget::getColor()
{
   return (m_outlinePen.color());
}

void NewCalendarWidget::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
   QCalendarWidget::paintCell(painter, rect, date);

   if (date == m_currentDate)
   {
       painter->setPen(m_outlinePen);
       painter->setBrush(m_transparentBrush);
       painter->drawRect(rect.adjusted(0, 0, -1, -1));
   }

}

void NewCalendarWidget::paintBirthDay(QDate date)
{
    QBrush brush;
    brush.setColor( Qt::green );
    QTextCharFormat cf = this->dateTextFormat( date );
    cf.setBackground( brush );
    this->setDateTextFormat( date, cf );

}

/*void NewCalendarWidget::clicked(QDate &date)
{
    QCalendarWidget::clicked(date);
}*/
