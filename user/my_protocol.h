#ifndef __MY_PROTOCOL_H_
#define __MY_PROTOCOL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#define FRAME_HEAD                        0xF5

#define CMD_SLAVE_NET_IN                  0xB0
#define CMD_MASTER_CTRL_SLAVE             0xB1

#define CMD_SHAKE_HAND                    0xD0


#define CMD_REPORT_SLAVE_STATE            0xC3

#define RET_SUCCESS                       0x00
#define RET_FAIL                          0x01
#define RET_CRC_ERROR                     0x02
#define RET_TIMEOUT                       0x03


//通信格式结构体
#define MAX_DATA_LEN        256
#define FRAME_HEAD_LEN      5
typedef union UN_COMMUNICATE_BUFF
{
    unsigned char databuff[MAX_DATA_LEN];
    struct
    {
        unsigned char head;
        unsigned char crc_hi;
        unsigned char crc_lo;
        unsigned char len;
        unsigned char cmd;
        unsigned char data[MAX_DATA_LEN-FRAME_HEAD_LEN];
    }Frame;
}TYP_UN_COMMUNICATE_BUFF;


class My_Protocol
{
public:
    My_Protocol();
    ~My_Protocol();

    static unsigned char protocol_data_package(unsigned char cmd,unsigned char len,unsigned char *p_in,TYP_UN_COMMUNICATE_BUFF *p_data);
    static unsigned char protocol_data_unpackage(TYP_UN_COMMUNICATE_BUFF *p_in,unsigned char *p_out);

private:

    static unsigned short get_crc(unsigned char *ptr,unsigned short len);
    static bool check_crc(TYP_UN_COMMUNICATE_BUFF *p_data);
};

#endif
