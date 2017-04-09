#-------------------------------------------------
#
# Project created by QtCreator 2017-04-07T23:21:04
#
#-------------------------------------------------

QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PrivacyAnalyzer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += WPCAP
DEFINES += HAVE_REMOTE

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES +=\
    overview.cpp \
    chart.cpp \
    main.cpp \
    capture.cpp

HEADERS  += overview.h \
    main.h \
    chart.h \
    capture.h

FORMS    += overview.ui

win32: LIBS += -L$$PWD/Winpcap/Lib/x64/ -lwpcap

INCLUDEPATH += $$PWD/Winpcap/Include
DEPENDPATH += $$PWD/Winpcap/Include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/Winpcap/Lib/x64/wpcap.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/Winpcap/Lib/x64/libwpcap.a

win32: LIBS += -lWS2_32

win32: LIBS += -liphlpapi

win32: LIBS += -lkernel32

target.path = $$[QT_INSTALL_EXAMPLES]/charts/dynamicspline
INSTALLS += target
