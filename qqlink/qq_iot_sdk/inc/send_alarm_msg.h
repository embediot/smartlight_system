#ifndef __SEND_ALARM_MSG_H_
#define __SEND_ALARM_MSG_H_



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <QString>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "my_header.h"

class Send_Alarm_Msg
{
public:
        Send_Alarm_Msg();          //构造方法
        ~Send_Alarm_Msg();        //析构方法

        void send_pic_to_mobile_qq();
        void send_audio_to_mobile_qq();
        void send_video_to_mobile_qq();

private:

        static void on_send_alarm_file_progress(const unsigned int cookie, unsigned long long transfer_progress, unsigned long long max_transfer_progress);
        static void on_send_alarm_msg_ret(const unsigned int cookie, int err_code);
};

#endif
