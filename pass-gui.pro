QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pass-gui
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES +=  main.cpp\
            mainwindow.cpp \
            pass-storage.cpp \
            windows-keyboard.cpp \
            encryptiongpg.cpp \
            wait-dialog.cpp

HEADERS +=  mainwindow.h \
            pass-storage.h \
            encryptiongpg.h \
            keyboard-interface.h \
            windows-keyboard.h \
            hotkey-setter.h \
            wait-dialog.h

FORMS += mainwindow.ui

LIBS += -luser32

DISTFILES += lock-logo.svg

win32 {
    DISTFILES += lock-logo.svg.ico
    RC_ICONS = lock-logo.svg.ico
}

RESOURCES += \
    icon.qrc
