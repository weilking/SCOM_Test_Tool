#-------------------------------------------------
#
# Project created by QtCreator 2018-05-04T21:23:06
#
#-------------------------------------------------

QT       += core gui
QT  += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SCOM_Test_Tool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cwidgetlogbox.cpp \
    cwidgetserialport.cpp \
    cscomworker.cpp \
    cwidgetscomeditor.cpp \
    cscomqueue.cpp \
    cwidgetcontrolpanel.cpp \
    cwidgetscomlist.cpp

HEADERS  += mainwindow.h \
    cwidgetlogbox.h \
    cwidgetserialport.h \
    cscomworker.h \
    common_inc.h \
    cwidgetscomeditor.h \
    cscomqueue.h \
    cwidgetcontrolpanel.h \
    cwidgetscomlist.h

DISTFILES += \
    Note.txt
