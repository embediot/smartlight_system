/*
 * TXCloudStorage.h
 * Author: realingzhou
 * Created: 2016-01-26
 */

 /////////////////////////////////////////////////////////////////////////
 //
 // >>>>>>>>> 音视频云存储接口 <<<<<<<<<<<
 //
 // (1) 此接口主要针对音视频数据流的存储解决方案，基本原则是复用QQ物联的音视频通道，并在
 //     腾讯QQ物联后台直接完成音视频数据流的文件落地。
 //
 // (2) 如果是文件或者图片，则并不适用该套接口，稍后的版本会提供专门针对文件和图片存储的
 //     对象级云存储接口。
 //
 // (3) 该套方案的优点是不占用额外的上行带宽，所以如果在云端视频录制过程中，并不影响手机
 //     终端的视频预览。但也有其不足指出：如果上行带宽不稳定，那么云端录制的视频会因为上
 //     传通道的不稳定而无法保证落地文件的质量。
 //
 // >>>>>>>>> 两种录制模式 <<<<<<<<<<<
 //
 // 目前录制模式分成两种，一种是24小时低清模式，一种是按需高清模式
 //
 // (1) 24小时低清模式: 全天24小时录制，适用于商用监控场景，码率要控制在500kbps以内，否则
 //     套餐费用会非常昂贵 （500kbps的全天录制文件约为5.4G）
 //
 // (2) 按需高清模式：如果配合pir传感器，在有热源经过时才进行录制，那么存储总时间一般不会
 //     太长，能在可以承受的价格范围内提供高清录制解决方案
 //
 // >>>>>>>>> 代码对接流程 - 说明 <<<<<<<<<<<
 //
 // (1) 首先需要说明一点，云存储服务并不是设备负责开启和关闭的，而是由用户在手机QQ上通过
 //     财付通支付购买的服务，服务目前按照日期和存储量分成多种套餐。
 //
 // (2) 本头文件中的接口并不能独立使用，需要配合TXAudioVideo.h中的音视频接口一起发挥作用
 //     因为QQ物联音视频云存储的机制是复用QQ物联的音视频通道。
 //
 // >>>>>>>>> 代码对接流程 - 编码 <<<<<<<<<<<
 // (1) 如果当前用户选择的套餐类型为 av_cloud_storage_full，云存储的启动和关闭由SDK来管理，
//		 SDK会自动触发 TXAudioVideo.h里的 on_start_camera 接口，并且自动启动云存储。对于设备，
//		on_watch_video_state_change告知设备当前是否有用户观看。当没有用户观看时，设备需要向
//		SDK填充码率较低（500kbps）的视频数据；当有用户观看时候，默认高清分辨率，此时设备不
//		应做码率限制。观看历史视频时SDK会停止云存储；切换到实时视频时，SDK会打开云存储
 //
 // (2) 如果当前用户选择的套餐类型为 av_cloud_storage_need，云存储的启动和关闭由设备来管理，
 //    码率方面无限制。观看历史视频时设备需要停止云存储；切换到实时视频时，设备需要打开云存储
 //
//  (3)  云存储  实时视频  历史视频 都复用同一个音视频通道，观看历史视频时不应云存储。当没有人观看时，
// 	设备需要停止播放历史视频。
//
 /////////////////////////////////////////////////////////////////////////

#ifndef TXCLOUDSTORAGE_H_
#define TXCLOUDSTORAGE_H_

#include "TXSDKCommonDef.h"

CXX_EXTERN_BEGIN

//云存储套餐类型
enum av_cloud_storage_type {
	av_cloud_storage_null   = 0 ,   // 当前设备未开通云存储
	av_cloud_storage_full   = 1,    // 7*24小时全量存储
	av_cloud_storage_need   = 2,    // 按需存储
};

/**
 * 云存储 回调接口
*/
typedef struct _tx_av_cloud_storage_notify
{
    /**
     * 当云存储套餐类型发生改变时候，此回调会将变动内容回调给设备
     *
     * param : old_state  旧套餐类型，取值参考 enum av_cloud_storage_type
	 * param : new_state 新套餐类型，取值参考 enum av_cloud_storage_type
     */
	void (*on_av_cloud_storage_type_change)(int old_state, int new_state);

    /**
     * 当摄像头的观看状态发生改变时候，此回调会将最新的观看状态回调给设备
     *
	 * param : state 观看状态，0表示无人观看，非0表示有人观看
     */
	void (*on_watch_video_state_change)(int state);
} tx_av_cloud_storage_notify;

/**
 * 云存储回调初始化
 * param: notify 设备回调通知
 */
SDK_API int tx_init_av_cloud_storage(tx_av_cloud_storage_notify* notify);

/**
 * 启动云存储， 套餐类型为av_cloud_storage_need时，调用
 */
SDK_API int tx_start_av_cloud_storage();

/**
 * 停止云存储， 套餐类型为av_cloud_storage_need时，调用
 */
SDK_API int tx_stop_av_cloud_storage();

CXX_EXTERN_END

#endif /* TXCLOUDSTORAGE_H_ */
