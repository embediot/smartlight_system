#ifndef __INIT_DATAPOINT_H_
#define __INIT_DATAPOINT_H_


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <QString>
#include <QDebug>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "my_header.h"


struct str_data_point
{
    unsigned char led1_state;          //led1状态
    unsigned char led2_state;          //led2状态
    unsigned char update_flag;        //更新状态
    unsigned char brightness;         //亮度

};

extern struct str_data_point data_point_send;

class Init_DataPoint  : public QObject
{
    Q_OBJECT
public:
    Init_DataPoint();          //构造方法
    ~Init_DataPoint();        //析构方法

    struct str_data_point data_point_send;

    int init_data_point();    //初始化数据点
    int update_temp_to_qq(unsigned long long from_id, tx_data_point  datapoint);      //向手机qq更新温度数据
    void set_led_state(unsigned long long from_id, tx_data_point  datapoint);    //设置led 状态
    void brightness_process(unsigned long long from_id, tx_data_point  datapoint);    //灯光亮度控制


signals:
    void signal_data_point(struct str_data_point data_point);

private:

    static void on_receive_datapoint(unsigned long long from_id, tx_data_point * data_points, int data_points_count);
    static void ret_call_msg(unsigned int cookie, int err_code);
    static void report_led_status(unsigned int cookie, int err_code);
    static void ask_led_status(unsigned int cookie, unsigned long long from_client, int err_code);
    static void ret_brightness(unsigned int cookie, int err_code);


};


#endif


