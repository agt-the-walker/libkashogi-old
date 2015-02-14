include(../defaults.pri)

CONFIG += testcase
QT += testlib

TEMPLATE = app

TARGET = testrunner

SOURCES += piecetest.cpp positiontest.cpp testrunner.cpp
HEADERS += piecetest.h positiontest.h

LIBS += ../src/libcore.a
