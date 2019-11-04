#-------------------------------------------------
#
# Project created by QtCreator 2017-09-11T10:31:19
#
#-------------------------------------------------

QT       += core gui serialport sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = smartlight_system
TEMPLATE = app

DEFINES += xHARDWARE_ENCDEC

SOURCES +=\
    user/main.cpp \
    user/welcomewidget.cpp \
    user/my_protocol.cpp \
    user/circle_queue.cpp \
    network/reg_dialog.cpp \
    network/network_widget.cpp \
    setting/setting_widget.cpp \
    setting/update_dialog.cpp \
    user/watchdogfeed.cpp \
    lamps/lamps_widget.cpp \
    qqlink/qqlink_widget.cpp \
    camera/camera_widget.cpp

HEADERS  += \
    user/welcomewidget.h \
    user/my_protocol.h \
    user/circle_queue.h \
    network/reg_dialog.h \
    network/network_widget.h \
    setting/setting_widget.h \
    setting/update_dialog.h \
    user/watchdogfeed.h \
    lamps/lamps_widget.h \
    qqlink/qqlink_widget.h \
    camera/camera_widget.h

FORMS    += welcomewidget.ui \
    network/reg_dialog.ui \
    setting/update_dialog.ui


RESOURCES += \
    resources/images.qrc

DISTFILES +=

