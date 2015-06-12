#-------------------------------------------------
#
# Project created by QtCreator 2015-06-12T21:52:14
#
#-------------------------------------------------

QT       -= gui

TARGET = pcrio
TEMPLATE = lib
CONFIG += staticlib

SOURCES += pcrio.cpp

HEADERS += pcrio.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
