#ifndef __INIT_DEVICE_H_
#define __INIT_DEVICE_H_



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


class Init_Device
{
public:
        Init_Device();          //构造方法
        ~Init_Device();        //析构方法

        int init_qq_device(void);           //初始化qq物联设备
        int exit_qq_device();                  //退出所有设备sdk相关逻辑


private:
        int   product_id;
        char  pubkey_file[128];
        char  guid_file[128];
        char  licence_file[128];

        bool read_buffer_from_file(char *file_path, unsigned char *p_buff, int in_size, int *size_used);   //读取文件到缓冲区
        void log_on();
        void log_off();

        int file_is_exist(char *file_path);     //判断文件是否可被访问
        int dev_config_init(void);                 //设备信息初始化

        static void on_login_complete(int errcode);
        static void on_online_status(int old_status, int new_status);
        static void log_func(int level, const char* module, int line, const char* message);


};



#endif

