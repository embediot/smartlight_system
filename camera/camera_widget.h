#ifndef __CAMERA_WIDGET_H_
#define __CAMERA_WIDGET_H_



#include <QWidget>
#include <QDebug>
#include <stdio.h>
#include <string.h>
#include "user/welcomewidget.h"
#include "ui_welcomewidget.h"

class WelcomeWidget;

class Camera_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Camera_Widget(WelcomeWidget *widget);
    ~Camera_Widget();



private slots:
    void slot_show_pixmap(QPixmap pixmap);

    void on_pb_open_camera_clicked();
    void on_pb_take_photos_clicked();
    void on_pb_take_videos_clicked();

private:

    WelcomeWidget *welcomewidget;
    Ui::WelcomeWidget *welcome_ui;

};



#endif

