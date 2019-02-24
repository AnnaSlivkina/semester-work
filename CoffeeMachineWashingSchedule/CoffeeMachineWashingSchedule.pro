#-------------------------------------------------
#
# Project created by QtCreator 2017-01-12T10:31:02
#
#-------------------------------------------------
HEADERS     = mainwindow.h \
    newcalendarwidget.h \
    database.h \
    formbd.h \
    timer.h
QT       += core

QT       += sql

QT       += widgets

TARGET = CoffeeMachineWashingSchedule
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    newcalendarwidget.cpp \
    database.cpp \
    formbd.cpp \
    timer.cpp

HEADERS += \
    mainwindow.h
