//
//  TXLanAudioVideo.hpp
//  txdevicesdk
//
//  Created by Rain on 16/3/18.
//  Copyright © 2016年 Tencent. All rights reserved.
//

#ifndef TXLanAudioVideo_h
#define TXLanAudioVideo_h
#include "TXSDKCommonDef.h"

CXX_EXTERN_BEGIN

/* TV跟camera做局域网音视频通信的接口，若不支持可以不实现
 */

//音频类型
enum lanav_audio_format {
    lanav_audio_format_pcm = 1,
};

/**
 * 收发音频数据流时的参数
 * head_length   : 结构体的长度.
 * audio_format  ：表示音频编码的格式，目前只支持PCM： 1
 * sampling_info ：采样参数 value = ((通道数 << 24) | (采样率 << 16) | (位宽 << 8)| 0x00)，建议值：GET_SIMPLING_INFO(1, 8, 16) = 17305600；
 *                 注  ：采样率 = 8000（8K） ，此处采样率请填8 ； 采样率  48000（48K） 此处填48； 以此类推。
 * reserved      ：保留字段
 */
typedef struct _tx_lanav_audio_encode_param
{
    unsigned char head_length;
    unsigned char audio_format;
    unsigned int  sampling_info;
    unsigned int  reserved;
    
} tx_lanav_audio_encode_param;


/**
 * 下面的宏来帮助您计算一个用来填入 tx_lanav_audio_encode_param - sampling_info 中的数值
 *  param： channel 声道个数
 *  param： sampling 采样率，采样率 = 8000（8K） ，此处采样率请填8 ； 采样率  48000（48K） 此处填48； 以此类推。
 *  param：bit 位宽，取 8 或 16，推荐16
 */
#define GET_LANAV_SIMPLING_INFO(channel, sampling, bit)  ((channel << 24) | (sampling << 16) | (bit << 8) | 0x00)


/**
 * AV SDK -> Device 回调接口
 *
 * on_start_camera 通知摄像头视频链路已经建立，可以通过 tx_lan_av_set_video_data 接口向 AV LANSDK 填充采集到的视频数据
 *
 * on_stop_camera  通知摄像头视频链路已经断开，可以不用再继续采集视频数据
 *
 * on_set_bitrate  视频码率意味着1s产生多少数据，这个参数跟网络带宽的使用直接相关。
 *                 AV SDK 会根据当前的网络情况和Qos信息，给出建议的bitrate，上层应用可以根据这个建议值设置Camera的各项参数，
 *                 如帧率、分辨率，量化参数等，从而获得合适的码率
 *
 * on_start_mic    通知麦克风音频链路已经建立，可以通过 tx_lan_av_set_audio_data 接口向 AV SDK 填充采集到的音频数据
 *
 * on_stop_mic     通知摄像头音频链路已经断开，可以不用再继续采集音频数据
 */
typedef struct _tx_lan_av_callback
{
    //智能设备作为音视频的发送方，下面回调用于采集音视频数据
    bool (*on_lanav_start_camera)();
    bool (*on_lanav_stop_camera)();
    
    bool (*on_lanav_set_bitrate)(int bit_rate);
    
    //目前此参数固定，仅支持PCM格式，16k采样率，单通道，位宽2Byte，通过tx_lan_av_set_audio_data传入的音频数据必须是此格式，否则处理不了
    bool (*on_lanav_start_mic)(tx_lanav_audio_encode_param* param);
    bool (*on_lanav_stop_mic)();
    
    /**
     * 强制编I帧，解决多人视频下，后进入房间的画面延迟大的问题。
     */
    void (*on_lanav_force_iframe)();
    
} tx_lan_av_callback;


/**
 * 接口说明：启动音视频相关服务，该服务需要登录成功后才能调用，否则会有错误码返回
 * 参数说明：callback  音视频回调接口
 * 返回值  ：错误码（见全局错误码表）
 */
SDK_API int tx_lanav_start_service(tx_lan_av_callback *callback);


/**
 * 接口说明：退出所有SDK音视频相关的逻辑
 * 返回值  ：错误码（见全局错误码表）
 */
SDK_API int tx_lanav_stop_service();


/**
 * 向SDK填充视频数据
 * nGopIndex:Gop的index
 * nFrameIndex:当前帧在所在gop中的index
 * nTotalIndex:当前帧在总编码过程中的index
 */
SDK_API void tx_lan_av_set_video_data(unsigned char *pcEncData, int nEncDataLen,
                               int nFrameType, int nTimeStamps, int nGopIndex, int nFrameIndex);

/**
 * 向SDK填充音频数据
 * param：param 音频编码参数
 * param：pcEncData 编码后的音频数据
 * param: nEncDataLen 编码的音频数据长度
 */
SDK_API void tx_lan_av_set_audio_data(tx_lanav_audio_encode_param *param, unsigned char *pcEncData, int nEncDataLen);

CXX_EXTERN_END

#endif /* TXLanAudioVideo_hpp */


