#-------------------------------------------------
#
# Project created by QtCreator 2014-10-10T20:00:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Meta1a
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    fileloader.cpp \
    greedy.cpp \
    localsearch.cpp \
    tabuelement.cpp \
    tabulist.cpp \
    tabusearch.cpp

HEADERS  += mainwindow.h \
    fileloader.h \
    greedy.h \
    localsearch.h \
    tabuelement.h \
    tabulist.h \
    tabusearch.h

FORMS    += mainwindow.ui
