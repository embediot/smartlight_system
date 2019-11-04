#ifndef __SERIAL_THREAD_H_
#define __SERIAL_THREAD_H_

#include <QObject>
#include <QMutex>
#include <QDebug>
#include <QThread>

#include "serial_port/serial_port.h"

class Serial_Thread : public QObject
{
    Q_OBJECT
public:
        Serial_Thread(Serial_Port* sp = NULL);
        ~Serial_Thread();


        void stop_thread();

signals:


public slots:
        void read_serial_port();

private:

        bool stop_flag;
        QMutex stop_mutex;
        Serial_Port *serial_port;
};





#endif
