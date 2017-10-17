#-------------------------------------------------
#
# Project created by QtCreator 2016-11-09T10:25:45
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = work_experience
TEMPLATE = app

RC_FILE = myapp.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    connectiondb.cpp \
    addrengedialog.cpp \
    listuser.cpp \
    finduser.cpp \
    calculation.cpp \
    calendar.cpp \
    fullmonth.cpp

HEADERS  += mainwindow.h \
    connectiondb.h \
    addrengedialog.h \
    listuser.h \
    finduser.h \
    calculation.h \
    calendar.h \
    fullmonth.h

FORMS    += mainwindow.ui

DISTFILES += \
    myapp.rc
