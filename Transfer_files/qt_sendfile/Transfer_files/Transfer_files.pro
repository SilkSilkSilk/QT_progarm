QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Transfer_files
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    wan_send.cpp \
    wan_sendfile.cpp \
    wan_recv.cpp


HEADERS  += mainwindow.h \
    wan_send.h \
    wan_sendfile.h \
    wan_recv.h


FORMS    += mainwindow.ui \
    wan_send.ui \
    wan_recv.ui

CONFIG += mobility
MOBILITY = 

