QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    loginscreen.cpp \
    main.cpp \
    mainwindow.cpp \
    signupscreen.cpp

HEADERS += \
    loginscreen.h \
    mainwindow.h \
    signupscreen.h

FORMS += \
    loginscreen.ui \
    mainwindow.ui \
    signupscreen.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
