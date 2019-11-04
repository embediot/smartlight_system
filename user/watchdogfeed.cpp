#include "watchdogfeed.h"




WatchDogFeed::WatchDogFeed(QObject *parent) : QObject(parent)
{
    udp = new QUdpSocket(this);
    //dest_addr = QHostAddress(DEST_IP);

    foreach(QHostAddress address,QNetworkInterface::allAddresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            local_addr = address;
        }
    }

    qDebug() << "smartlight app local ip = " << local_addr;

    dest_addr = local_addr;

    udp->bind(local_addr, LOCAL_PORT);

    connect(udp, SIGNAL(readyRead()),this, SLOT(read_udp_socket_data()));
}




void WatchDogFeed::onSuspend(quint8 sec)
{    
    QByteArray data_send;
    unsigned char data_len = 0;
    unsigned char data_buff[1];
    TYP_UN_COMMUNICATE_BUFF package_data;

    data_buff[0] = sec;
    data_len = My_Protocol::protocol_data_package(SYSCMD_SUSPEND,1,data_buff,&package_data);

    data_send = QByteArray((char*)&package_data,data_len);
    data_send.resize(data_len);

    udp->writeDatagram(data_send.data(), data_send.size(),dest_addr, DEST_PORT);
}

void WatchDogFeed::onStop()
{   
    QByteArray data_send;
    unsigned char data_len = 0;
    unsigned char data_buff[1];
    TYP_UN_COMMUNICATE_BUFF package_data;

    data_buff[0] = 0xff;
    data_len = My_Protocol::protocol_data_package(SYSCMD_STOP,1,data_buff,&package_data);

    data_send = QByteArray((char*)&package_data,data_len);
    data_send.resize(data_len);

    udp->writeDatagram(data_send.data(), data_send.size(),dest_addr, DEST_PORT);
}

void WatchDogFeed::onResume()
{    
    QByteArray data_send;
    unsigned char data_len = 0;
    unsigned char data_buff[1];
    TYP_UN_COMMUNICATE_BUFF package_data;

    data_buff[0] = 0xff;
    data_len = My_Protocol::protocol_data_package(SYSCMD_RESUME,1,data_buff,&package_data);

    data_send = QByteArray((char*)&package_data,data_len);
    data_send.resize(data_len);

    udp->writeDatagram(data_send.data(), data_send.size(),dest_addr, DEST_PORT);
}

void WatchDogFeed::onRestartApp()
{
    QByteArray data_send;
    unsigned char data_len = 0;
    unsigned char data_buff[1];
    TYP_UN_COMMUNICATE_BUFF package_data;

    data_buff[0] = 0xff;
    data_len = My_Protocol::protocol_data_package(SYSCMD_APPRESTART,1,data_buff,&package_data);

    data_send = QByteArray((char*)&package_data,data_len);
    data_send.resize(data_len);

    udp->writeDatagram(data_send.data(), data_send.size(),dest_addr, DEST_PORT);
}


void WatchDogFeed::onUpgradeAPP(char flag)
{           
    QByteArray data_send;
    unsigned char data_len = 0;
    unsigned char data_buff[256];
    TYP_UN_COMMUNICATE_BUFF package_data;


    if(flag == NET_UPGRADE)
    {
        QString url = UPGRADE_URL;

        data_buff[0] = flag;
        memcpy(&data_buff[1],url.toLatin1().data(),url.length());

        data_len = My_Protocol::protocol_data_package(SYSCMD_APPUPGRADE,1+url.length(),data_buff,&package_data);

        data_send = QByteArray((char*)&package_data,data_len);
        data_send.resize(data_len);
    }
    else
    {
        data_buff[0] = flag;
        data_len = My_Protocol::protocol_data_package(SYSCMD_APPUPGRADE,1,data_buff,&package_data);

        data_send = QByteArray((char*)&package_data,data_len);
        data_send.resize(data_len);
    }

    udp->writeDatagram(data_send.data(), data_send.size(),dest_addr, DEST_PORT);

}

void WatchDogFeed::read_udp_socket_data()
{
    while(udp->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udp->pendingDatagramSize());
        udp->readDatagram(datagram.data(), datagram.size());

        //qDebug() << datagram.toHex();

    }
}

