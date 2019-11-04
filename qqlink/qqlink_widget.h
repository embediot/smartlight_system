#ifndef __QQLINK_WIDGET_H_
#define __QQLINK_WIDGET_H_

#include <QWidget>
#include <QDebug>
#include <stdio.h>
#include <string.h>
#include "user/welcomewidget.h"
#include "ui_welcomewidget.h"

class WelcomeWidget;



class QQLink_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit QQLink_Widget(WelcomeWidget *widget);
    ~QQLink_Widget();

private slots:

    void on_pb_link_qq_server_clicked();

private:


    WelcomeWidget *welcomewidget;
    Ui::WelcomeWidget *welcome_ui;

};


#endif


