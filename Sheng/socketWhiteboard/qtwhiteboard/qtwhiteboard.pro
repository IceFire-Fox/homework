QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ./include

LIBS += -I./include

SOURCES += \
    drawspace.cpp \
    main.cpp \
    drawwindow.cpp \
    src/bezier.cpp \
    src/linkedlist.c

HEADERS += \
    drawspace.h \
    drawwindow.h \
    ./include/bezier.h \
    ./include/linkedlist.h

FORMS += \
    drawwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
