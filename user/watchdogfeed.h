#ifndef __WATCHDOGFEED_H_
#define __WATCHDOGFEED_H_

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QtNetwork>

#include "my_protocol.h"


#define DEST_IP       "127.0.0.0"
#define DEST_PORT     6899

#define LOCAL_IP      "127.0.0.0"
#define LOCAL_PORT    6799


#define SYSCMD_APPRESTART          0x10
#define SYSCMD_APPUPGRADE          0x11
#define SYSCMD_HANDSHAKE           0x12
#define SYSCMD_FEEDDOG             0x20
#define SYSCMD_SUSPEND             0x30
#define SYSCMD_RESUME              0x31
#define SYSCMD_STOP                0x40
#define SYSCMD_START               0x41
#define SYSCMD_CLEAR               0x00

#define USB_UPGRADE                0x01
#define NET_UPGRADE                0x02

#define UPGRADE_URL "http://192.168.1.123/smartlight/smartlight_system"

class WatchDogFeed : public QObject
{
    Q_OBJECT
public:
    explicit WatchDogFeed(QObject *parent = 0);
    void onSuspend(quint8 sec);
    void onResume();
    void onRestartApp();
    void onStop();
    void onUpgradeAPP(char flag);
signals:

public slots:
    void read_udp_socket_data();

private:
    QHostAddress dest_addr;
    QHostAddress local_addr;
    QUdpSocket *udp;
};


#endif

