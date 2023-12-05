#-------------------------------------------------
#
# Project created by QtCreator 2018-10-26T21:45:23
#
#-------------------------------------------------
QT+= sql
QT       += core gui sql
QT       += core gui sql network multimedia multimediawidgets charts printsupport widgets axcontainer
QT += printsupport serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets quickwidgets
QT +=network
TARGET = Atelier_Connexion
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

CONFIG += c++11

SOURCES += \
    Sms.cpp \
    arduino.cpp \
    facture.cpp \
    fournisseur.cpp \
        main.cpp \
    connection.cpp \
    mainwindow_f.cpp \
    mainwindow_fac.cpp \
    menu.cpp

HEADERS += \
    Sms.h \
    arduino.h \
    facture.h \
    fournisseur.h \
    connection.h \
    mainwindow_f.h \
    mainwindow_fac.h \
    menu.h

FORMS += \
        mainwindow_f.ui \
        mainwindow_fac.ui \
        menu.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qml.qrc
