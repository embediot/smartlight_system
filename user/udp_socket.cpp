#include "udp_socket.h"

Udp_Socket::Udp_Socket()
{
    receiver_address = "";
    socket_descriptor = 0;
}

bool Udp_Socket::send_data(QByteArray data, struct sockaddr_in server_address)
{
    if(sendto(socket_descriptor,data.data(),data.length(),0,(struct sockaddr *)&server_address,sizeof(server_address)) == -1)
        return false;
    return true;
}

