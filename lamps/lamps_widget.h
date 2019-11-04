#ifndef __LAMPS_WIDGET_H_
#define __LAMPS_WIDGET_H_

#include <QWidget>
#include <QDebug>
#include <stdio.h>
#include <string.h>
#include "user/welcomewidget.h"
#include "ui_welcomewidget.h"


class WelcomeWidget;

class Lamps_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Lamps_Widget(WelcomeWidget *widget);
    ~Lamps_Widget();

private slots:

    void on_pb_all_lamps_ctrl_clicked();
    void on_pb_brightness_decrease_clicked();
    void on_pb_brightness_increase_clicked();

    void on_horizontalSlider_brightness_valueChanged(int value);

private:

    WelcomeWidget *welcomewidget;
    Ui::WelcomeWidget *welcome_ui;

};


#endif

