#ifndef __INIT_FILE_TRANSFER_H_
#define __INIT_FILE_TRANSFER_H_


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

class Init_File_Transfer
{
public:
        Init_File_Transfer();          //构造方法
        ~Init_File_Transfer();        //析构方法

        int init_file_transfer(void);           //初始化文件传输接口


private:
        static void cb_on_transfer_progress(unsigned long long transfer_cookie, unsigned long long transfer_progress, unsigned long long max_transfer_progress);
        static void cb_on_recv_file(unsigned long long transfer_cookie, const tx_ccmsg_inst_info * inst_info, const tx_file_transfer_info * tran_info);
        static void cb_on_transfer_complete(unsigned long long transfer_cookie, int err_code, tx_file_transfer_info* tran_info);
};

#endif


