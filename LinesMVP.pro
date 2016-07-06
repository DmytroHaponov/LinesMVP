#-------------------------------------------------
#
# Project created by Haponov Dmytro
#
#-------------------------------------------------

QT       += core gui widgets
qtHaveModule(printsupport): QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LinesMVP
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    paintarea.cpp

HEADERS  += mainwindow.h \
    paintarea.h

FORMS    += mainwindow.ui
