#ifndef __SETTING_WIDGET_H_
#define __SETTING_WIDGET_H_


#include <QWidget>
#include <QDebug>
#include <stdio.h>
#include <string.h>
#include <QProcess>
#include "user/welcomewidget.h"
#include "ui_welcomewidget.h"
#include "setting/update_dialog.h"


class WelcomeWidget;





#define OPEN_DOOR    0x01    //开门
#define CLOSE_DOOR   0x02    //关门

#define SERIAL_PORT_TX    0x01
#define SERIAL_PORT_RX    0x02

class Setting_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Setting_Widget(WelcomeWidget *widget);
    ~Setting_Widget();

private slots:
    void on_pb_update_clicked();
    void on_pb_time_update_clicked();
    void slot_time_update_finished(int exit_code, QProcess::ExitStatus exit_status);


private:

    WelcomeWidget *welcomewidget;
    Ui::WelcomeWidget *welcome_ui;
};

#endif
