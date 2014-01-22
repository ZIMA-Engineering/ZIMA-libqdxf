QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testapp
TEMPLATE = app

LIBS += -L../  \
    -lqdxf \

SOURCES += main.cpp \


