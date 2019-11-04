#include "serial_thread.h"

Serial_Thread::Serial_Thread(Serial_Port* sp)
{
    stop_flag = true;
    this->serial_port = sp;
}

Serial_Thread::~Serial_Thread()
{

}

void Serial_Thread::read_serial_port()
{
    qDebug() << "read_serial_port";
    {
        QMutexLocker locker(&stop_mutex);
        stop_flag = false;
    }

    while(1)
    {
        {
            QMutexLocker locker(&stop_mutex);
            if(stop_flag)
            return;
        }


    }
}

void Serial_Thread::stop_thread()
{
    {
        QMutexLocker locker(&stop_mutex);
        stop_flag = true;
    }
}
