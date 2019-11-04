#include "qqlink/qq_iot_sdk/inc/send_alarm_msg.h"

using namespace std;


//构造方法
Send_Alarm_Msg::Send_Alarm_Msg()
{

}

//析构方法
Send_Alarm_Msg::~Send_Alarm_Msg()
{

}

/**
 * 这段代码的目的是用来演示：设备向手q发送报警消息,主要演示发送图文报警和语音报警
 * 报警消息实际上是动态消息，首先需要在iot.qq.com上注册和关联动态消息，这样物联后台才能正确转发报警消息
 * 具体配置情参考物联文档的第5.3章节：http://iot.open.qq.com/wiki/index.html#!CASE/IP_Camera.md
 */


void Send_Alarm_Msg::on_send_alarm_file_progress(const unsigned int cookie, unsigned long long transfer_progress, unsigned long long max_transfer_progress)
{
    (void)transfer_progress;
    (void)max_transfer_progress;

    printf("on_send_alarm_file_progress, cookie[%u]\n", cookie);
}

void Send_Alarm_Msg::on_send_alarm_msg_ret(const unsigned int cookie, int err_code)
{
    printf("on_send_alarm_msg_ret, cookie[%u] ret[%d]\n", cookie, err_code);
}

/*
 * 发送图文报警，假设您已经在物联官网上配置触发器值为1时发送图文报警
 * 注意，显示顺序为：
 *		发现异常
 *		缩略图
 *		客厅发现异常
 *      点击查看，点击图片任意一处，
 */
void Send_Alarm_Msg::send_pic_to_mobile_qq()
{
    // 发送图片的结构化消息
    structuring_msg msg;

    bzero(&msg,sizeof(structuring_msg));

    msg.msg_id = 2;   //假设您已经在物联官网上配置触发器值为1时发送图文报警
    msg.file_path = (char*)"/opt/homekit/photos/123.png";               //文件的path
    msg.thumb_path = (char*)"./res/thumb.png";//缩略图path
    msg.title = (char*)"发现异常";
    msg.digest = (char*)"客厅发现异常";
    msg.guide_words = (char*)"点击查看";

    tx_send_msg_notify notify ;
    bzero(&notify,sizeof(tx_send_msg_notify));

    notify.on_file_transfer_progress = this->on_send_alarm_file_progress;
    notify.on_send_structuring_msg_ret = this->on_send_alarm_msg_ret;
    tx_send_structuring_msg(&msg, &notify, 0);
}

/*
 * 发送语音报警，假设您已经在物联官网上配置触发器值为3时发送语音报警
 */
void Send_Alarm_Msg::send_audio_to_mobile_qq()
{

    // 发送语音的结构化消息
    structuring_msg msg;

    bzero(&msg,sizeof(structuring_msg));

    msg.msg_id = 3;          //假设您已经在物联官网上配置触发器值为2时发送语音报警
    msg.file_path = (char*)"./res/100ask_test.mp3";
    msg.title = (char*)"语音报警";
    msg.digest = (char*)"收到语音报警";
    msg.guide_words = (char*)"点击查看";

    tx_send_msg_notify notify;

    bzero(&notify,sizeof(tx_send_msg_notify));

    notify.on_file_transfer_progress = this->on_send_alarm_file_progress;
    notify.on_send_structuring_msg_ret = this->on_send_alarm_msg_ret;
    tx_send_structuring_msg(&msg, &notify, 0);

}

/*
 * 发送视频报警，假设您已经在物联官网上配置触发器值为4时发送语音报警
 */
void Send_Alarm_Msg::send_video_to_mobile_qq()
{
    // 发送视频的结构化消息
    structuring_msg msg;

    bzero(&msg,sizeof(structuring_msg));

    msg.msg_id = 4;             //假设您已经在物联官网上配置触发器值为2时发送语音报警
    msg.file_path = (char*)"/opt/homekit/videos/my_video.mp4";
    msg.title = (char*)"视频警告";
    msg.digest = (char*)"收到视频报警";
    msg.guide_words = (char*)"点击查看";

    tx_send_msg_notify notify;
    bzero(&notify,sizeof(tx_send_msg_notify));

    notify.on_file_transfer_progress = this->on_send_alarm_file_progress;
    notify.on_send_structuring_msg_ret = this->on_send_alarm_msg_ret;
    tx_send_structuring_msg(&msg, &notify, 0);

}



