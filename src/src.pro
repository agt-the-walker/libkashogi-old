include(../defaults.pri)

CONFIG += staticlib

TEMPLATE = lib

TARGET = core

SOURCES += piece.cpp position.cpp
HEADERS += global.h piece.h position.h
