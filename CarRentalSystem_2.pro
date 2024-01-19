QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    basebar.cpp \
    favouritecartypesbar.cpp \
    historyscreen.cpp \
    lastrentalbar.cpp \
    loginscreen.cpp \
    main.cpp \
    mainscreen.cpp \
    mainwindow.cpp \
    newtripscreen.cpp \
    signupscreen.cpp \
    topbrandsbar.cpp

HEADERS += \
    basebar.h \
    favouritecartypesbar.h \
    historyscreen.h \
    lastrentalbar.h \
    loginscreen.h \
    mainscreen.h \
    mainwindow.h \
    newtripscreen.h \
    signupscreen.h \
    topbrandsbar.h

FORMS += \
    historyscreen.ui \
    loginscreen.ui \
    mainscreen.ui \
    mainwindow.ui \
    newtripscreen.ui \
    signupscreen.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
