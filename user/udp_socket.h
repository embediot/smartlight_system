#ifndef __UDP_SOCKET_H_
#define __UDP_SOCKET_H_


#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <QObject>
#include <arpa/inet.h>
#include <QMutex>

using namespace std;


#define SERVER_IP              "127.0.0.1"
#define SERVER_PORT            6899
#define LOCAL_PORT             6799
#define LOCAL_IP               "127.0.0.1"


class Udp_Socket : public QObject
{
    Q_OBJECT

public:
    Udp_Socket();
    bool create_socket()
    {
        if((socket_descriptor = socket(AF_INET,SOCK_DGRAM,0)) == -1)      //IPV4  SOCK_DGRAM 数据报套接字（UDP协议）
            return false;
        return true;
    }

    bool bind_local_socket(string ip, int port_num)
    {
        m_local_address = init_sockaddr(ip, port_num);
        if(socket_descriptor == 0 && !create_socket())return false;
        if(bind(socket_descriptor,(struct sockaddr *)&m_local_address,sizeof(m_local_address)) == -1)return false;
        return true;
    }

    int get_socket_descriptor()
    {
        return socket_descriptor;
    }

    string get_receiver_address()
    {
        return receiver_address;
    }

    bool close_socket()
    {
        close(socket_descriptor);
        socket_descriptor = 0;
        return true;
    }

    struct sockaddr_in init_sockaddr(string address, int port_num)
    {
        struct sockaddr_in sockaddr;
        bzero(&sockaddr,sizeof(sockaddr));
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_addr.s_addr = inet_addr(address.c_str());    //这里不一样
        sockaddr.sin_port=htons(port_num);
        return sockaddr;
    }

    bool send_data(QByteArray data, struct sockaddr_in server_address);
    void recv_data();

    sockaddr_in get_address()
    {
        return m_local_address;
    }


private:
    int socket_descriptor; //本地套接口描述字
    struct sockaddr_in m_local_address;//本地的地址
    struct sockaddr_in m_server_address;//服务器的地址
    string receiver_address;
};


#endif
