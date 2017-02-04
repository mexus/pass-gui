QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pass-gui
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES +=  main.cpp\
            lib/pass-storage.cpp \
            lib/win/windows-keyboard.cpp \
            gui/wait-dialog.cpp \
            gui/mainwindow.cpp

HEADERS +=  lib/pass-storage.h \
            lib/encryptiongpg.h \
            lib/keyboard-interface.h \
            lib/hotkey-setter.h \
            gui/wait-dialog.h \
	    gui/mainwindow.h

FORMS += gui/mainwindow.ui

LIBS += -luser32

DISTFILES += gui/lock-logo.svg

win32 {
    SOURCES += lib/win/encryptiongpg-windows.cpp
    HEADERS += lib/win/windows-keyboard.h
    DISTFILES += gui/lock-logo.svg.ico
    RC_ICONS = gui/lock-logo.svg.ico
}

RESOURCES += \
    icon.qrc
