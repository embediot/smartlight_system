#include "qqlink/qq_iot_sdk/inc/init_file_transfer.h"

using namespace std;


//构造方法
Init_File_Transfer::Init_File_Transfer()
{

}

//析构方法
Init_File_Transfer::~Init_File_Transfer()
{

}

/**
 * 这段代码的目的是用来演示：手Q向设备发送语音消息
 *
 * 手Q发送的语音消息实际上就是发送了一个AMR格式的文件。
 * 首先确保手Q与设备已经绑定，通过tx_init_file_transfer函数设置文件传输相关的回调函数。
 *
 * 期望的结果是：
 * 设备收到文件后，ontransfercomplete函数中将有文件信息的打印，
 * tx_init_file_transfer第2个参数是接收文件的目录，该目录下将存在刚刚收到的文件。
 * 当然您也可以通过tx_send_file_to发送文件（视频，语音，图片等）给手Q。
 */

void Init_File_Transfer::cb_on_transfer_progress(unsigned long long transfer_cookie, unsigned long long transfer_progress, unsigned long long max_transfer_progress)
{
        (void)transfer_cookie;

        printf("========> on file progress %f%%\n", transfer_progress * 100.0 / max_transfer_progress);
}

// 收到C2C transfer通知
void Init_File_Transfer::cb_on_recv_file(unsigned long long transfer_cookie, const tx_ccmsg_inst_info * inst_info, const tx_file_transfer_info * tran_info)
{
        (void)transfer_cookie;
        (void)inst_info;
        (void)tran_info;
}

/**
 * 文件传输完成后的结果通知，成功接收到文件后，厂商调用自己的接口播放语音文件
 */
void Init_File_Transfer::cb_on_transfer_complete(unsigned long long transfer_cookie, int err_code, tx_file_transfer_info* tran_info)
{
    printf("================ontransfer complete=====transfer_cookie == %lld ====================\n", transfer_cookie);
    // 这个 transfer_cookie 是SDK内部执行文件传输（接收或发送） 任务 保存的一个标记，在回调完这个函数后，transfer_cookie 将失效
    // 可以根据 transfer_cookie 查询文件的信息

    //在完成回调里直接传回 tran_info
    // tx_file_transfer_info ftInfo = {0};
    // tx_query_transfer_info(transfer_cookie, &ftInfo);

    printf("errcode %d, bussiness_name [%s], file path [%s]\n", err_code, tran_info->bussiness_name,  tran_info->file_path);
    printf("===============================================================================\n");
    if(err_code == 0)
    {
        //如果是语音文件，则播放，否则不处理
        if(strcmp(tran_info->bussiness_name, BUSINESS_NAME_AUDIO_MSG) == 0)
        {
                //do_play(tran_info->file_path);
        }
        else
        {
        }
    }
}

//初始化文件传输接口
int Init_File_Transfer::init_file_transfer(void)
{
        int ret = -1;

        //收到文件（一般是手q向设备发送的语音文件）的事件通知
        tx_file_transfer_notify fileTransferNotify;
        bzero(&fileTransferNotify,sizeof(tx_file_transfer_notify));

        fileTransferNotify.on_transfer_complete = cb_on_transfer_complete;
        fileTransferNotify.on_transfer_progress = cb_on_transfer_progress;
        fileTransferNotify.on_file_in_come 		= cb_on_recv_file;

        ret = tx_init_file_transfer(fileTransferNotify, (char*)"./tmp/ramdisk/");

        return true;
}
