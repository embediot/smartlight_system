#ifndef __NETWORK_WIDGET_H_
#define __NETWORK_WIDGET_H_


#include <QWidget>
#include <stdio.h>
#include <string.h>
#include <QDebug>
#include "user/welcomewidget.h"
#include "ui_welcomewidget.h"
#include "network/reg_dialog.h"

class WelcomeWidget;


#define OPEN_DOOR    0x01    //开门
#define CLOSE_DOOR   0x02    //关门

#define SERIAL_PORT_TX    0x01
#define SERIAL_PORT_RX    0x02

class Network_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Network_Widget(WelcomeWidget *widget);
    ~Network_Widget();

private slots:

    void on_pb_register_clicked();
    void on_pb_clear_tx_clicked();
    void on_pb_clear_rx_clicked();

private:

    WelcomeWidget *welcomewidget;
    Ui::WelcomeWidget *welcome_ui;

    void control_relay(unsigned char door_num,unsigned char ctrl_flag);
    void display_serial_port_data(char *p_data, int len, char flag);
};

#endif


