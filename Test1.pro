#-------------------------------------------------
#
# Project created by QtCreator 2015-10-24T09:21:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mandelbrot
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp \
    tools.cpp

HEADERS  += Fenster.hpp \
    Grafik.hpp \
    ../../Eclipse Projects/Git/MandelbrotLib/src/Complex.hpp \
    ../../Eclipse Projects/Git/MandelbrotLib/src/Mandelbrot.hpp \
    Complex.hpp \
    Mandelbrot.hpp \
    GrafikView.hpp \
    tools.hpp

FORMS    += fenster.ui

RESOURCES += \
    res.qrc
