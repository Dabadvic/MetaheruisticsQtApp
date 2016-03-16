#-------------------------------------------------
#
# Project created by QtCreator 2014-10-10T20:00:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Meta2
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    fileloader.cpp \
    greedy.cpp \
    localsearch.cpp \
    grasp.cpp \
    ils.cpp \
    agg.cpp \
    age.cpp \
    chromosome.cpp

HEADERS  += mainwindow.h \
    fileloader.h \
    greedy.h \
    localsearch.h \
    grasp.h \
    ils.h \
    agg.h \
    age.h \
    chromosome.h

FORMS    += mainwindow.ui
