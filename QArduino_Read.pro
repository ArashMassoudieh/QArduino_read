#-------------------------------------------------
#
# Project created by QtCreator 2018-11-04T10:29:29
#
#-------------------------------------------------

QT       += core gui
INCLUDEPATH += ./Serial/include
INCLUDEPATH += ./qcustomplot/

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql printsupport

TARGET = QArduino_Read
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    Serial/src/serial.cc \
    Serial/src/impl/unix.cc \
    Serial/src/impl/list_ports/list_ports_linux.cc \
    connectmysql.cpp \
    qcustomplot/qcustomplot.cpp \
    showhistoricdata.cpp

HEADERS += \
        mainwindow.h \
    connectmysql.h \
    qcustomplot/qcustomplot.h \
    showhistoricdata.h

FORMS += \
        mainwindow.ui \
    showhistoricdata.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
