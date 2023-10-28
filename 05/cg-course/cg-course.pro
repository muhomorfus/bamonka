QT       += core gui testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/branch.cpp \
    src/forest.cpp \
    src/intenser.cpp \
    src/light.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/phongintenser.cpp \
    src/polygon.cpp \
    src/scene.cpp \
    src/simpleintenser.cpp \
    src/sphereshell.cpp \
    unit_tests/testbranch.cpp \
    unit_tests/testforest.cpp \
    unit_tests/testpolygon.cpp

HEADERS += \
    inc/branch.h \
    inc/forest.h \
    inc/intenser.h \
    inc/light.h \
    inc/mainwindow.h \
    inc/phongintenser.h \
    inc/polygon.h \
    inc/ray.h \
    inc/scene.h \
    inc/simpleintenser.h \
    inc/sphereshell.h \
    unit_tests/testbranch.h \
    unit_tests/testforest.h \
    unit_tests/testpolygon.h

FORMS += \
    gui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
