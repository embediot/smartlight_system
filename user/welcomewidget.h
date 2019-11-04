#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include <QWidget>
#include <stdio.h>
#include <string.h>
#include <QDebug>
#include <QDateTime>
#include <QVBoxLayout>
#include <QTimer>

#include "circle_queue.h"
#include "user/watchdogfeed.h"


#include "network/network_widget.h"
#include "setting/setting_widget.h"
#include "lamps/lamps_widget.h"
#include "qqlink/qqlink_widget.h"
#include "camera/camera_widget.h"


namespace Ui {
class WelcomeWidget;
}

class Network_Widget;
class Setting_Widget;
class Lamps_Widget;
class QQLink_Widget;
class Camera_Widget;

#define SOFTWARE_VERSION     "V 1.6.8"
#define HARDWARE_VERSION     "V 1.6.8"

#define COMPANY_INFO         "EmbedIoT"


#define TIMEOUT_BASE_VALUE           100                  //200ms
#define TIMEOUT_CURRTIME_UPDATE      1000                 //1000ms

class WelcomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeWidget(QWidget *parent = 0);
    ~WelcomeWidget();
    Ui::WelcomeWidget *ui;


private slots:
    void basetimer_timeout_handler();

    void on_pb_lamps_clicked();
    void on_pb_camera_clicked();
    void on_pb_qqlink_clicked();
    void on_pb_network_clicked();
    void on_pb_setting_clicked();



private:



    QTimer *base_timer;
    WatchDogFeed *watchdog;
    Network_Widget *network_widget;
    Setting_Widget *setting_widget;
    Lamps_Widget *lamps_widget;
    QQLink_Widget *qqlink_widget;
    Camera_Widget *camera_widget;

    void update_current_time();
    void update_information();

};

#endif // WELCOMEWIDGET_H
