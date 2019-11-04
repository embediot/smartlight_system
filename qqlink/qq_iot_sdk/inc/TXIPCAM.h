#ifndef __IPCAMERA_H__
#define __IPCAMERA_H__

#include "TXSDKCommonDef.h"
#include "TXDeviceSDK.h"

CXX_EXTERN_BEGIN


/////////////////////////////////////////////////////////////////
//
//         摄像头 清晰度调整 & 云台控制 
//
/////////////////////////////////////////////////////////////////


// 视频清晰度 定义
enum definition {
	def_low		= 1,	//低
	def_middle	= 2,	//中
	def_high	= 3,	//高
};


// 云台(PTZ)旋转方向
enum rotate_direction {
	rotate_direction_left	= 1,	//左
	rotate_direction_right	= 2,	//右
	rotate_direction_up		= 3,	//上
	rotate_direction_down	= 4,	//下
};

// 云台(PTZ)旋转角度范围
enum rotate_degree {
	//水平角度范围
	rotate_degree_h_min = 0,
	rotate_degree_h_max = 360,

	//垂直角度范围
	rotate_degree_v_min = 0,
	rotate_degree_v_max = 180,
};

// 回调处理函数，需要产商实现。
typedef struct _tx_ipcamera_notify {
	/**
	 * 接口说明:	ipcamera清晰度调整回调，调用后返回摄像头的当前分辨率
	 * param : definition :  要调整的清晰度，具体取值参见 enum definition
	 * param : cur_definition：暂时保留，没有用到
	 * param : cur_definition_length : 暂时保留，没有用到。
	*/
	int (*on_set_definition)(int definition, char *cur_definition, int cur_definition_length);

	/**
	 * 接口说明：ipcamera云台控制回调
	 * param ：rotate_direction : 云台调整方向，具体取值可以参见 enum rotate_direction
	 * param ：rotate_degree ： 云台调整角度，具体取值可以参见 enum rotate_degree
	 * return : 0代表调整失败， 非0代表调整成功。
	*/
	int (*on_control_rotate)(int rotate_direction, int rotate_degree);

	/**
	 * 接口说明：ipcamera变倍控制回调
	 * param zoom			 :  放大或者缩小, 负值表缩小，负值代表放大。
	 * return : 0代表调整失败， 非0代表调整成功。
	*/
	int (*on_control_zoom)(int zoom);
} tx_ipcamera_notify;


/**
 * 接口说明： 设置ipcamera相关的回调
 * param: notify IPCamera交互回调对象。
 */
SDK_API int tx_ipcamera_set_callback(tx_ipcamera_notify *notify);



/////////////////////////////////////////////////////////////////
//
//         ipcamera互联相关接口: 历史视频回看
//
/////////////////////////////////////////////////////////////////

/**
  *  QQ物联历史视频回看方案：
  *
  *      |-----------|                                        |—————————————|
  *      |           |    <---- 1.on_fetch_history_video --   |             |
  *      |  DEVICE   |                                        |     APP     |
  *      |           |    <---- 2.on_play_history_video ----  |             |
  *      |           |                                        |             |
  *      |-----------|                                        |—————————————|
  *
  *    重要：如果需要使用历史视频回看功能，必须在iot后台开启“是否支持历史视频回看”功能（功能id：200021），否则手机QQ不会显示历史视频的时间轴
  *
  *    step1. 手机QQ在视频连接成功之后向设备查询历史视频纪录用来绘制历史视频的时间轴，所以需要设备本地存储历史视频的文件和文件对应的时间段
  *    step2. 用户在手机QQ上选择历史视频，手机QQ向设备发送播放历史视频命令，需要使用 tx_set_history_video_data/tx_set_history_audio_data,
  *            发送历史音视频数据。
  *      
  *      #########################################################################
  *      ##                                                                     ##
  *      ##  注意：                                                             ##
  *      ##    1.手Q请求完历史视频，不再自动触发下一个分段的视频请求。          ##
  *      ##    2.设备需要自己处理视频播放，保证视频流不会断掉。推荐做法，当视频 ##
  *      ##      片段播放结束，如果还有继续播下一个，否则切到实时，遇到视频帧错 ##
  *      ##      误等异常情况，需要有容错处理，这时推荐切到实时。               ##
  *      ##                                                                     ##
  *      #########################################################################
  * 
  *  手Q中历史视频时间轴示意图:
  *           历史视频片段2                历史视频片段1                  实时视频
  *      ｜----------------------------------------------------------------|
  *      ｜   ||||||||||||||              ||||||||||||||                   |
  *      ｜   ||||||||||||||              ||||||||||||||                   |
  *      ｜----------------------------------------------------------------|
  *           10:30     11:00             12:00     12:30               14:00
  */
  
/**
 * 本地历史视频 片段区间信息
 */
typedef struct _tx_history_video_range
{
	unsigned int start_time;     //视频片段开始时间 单位s.
	unsigned int end_time;       //视频片段结束时间 单位s.
} tx_history_video_range;


// 交互处理函数，由产商实现。
typedef struct _tx_history_video_notify
{
	/**
	 * 接口说明: 查询历史视频信息 step1
	 * param: channel     多路设备用于区分通道，单通道设备可以忽略。默认值为1. 手Q6.5.5支持
	 * param: last_time   手机QQ分段拉取历史视频信息，last_time为时间轴末端的时间(unix时间戳），例如时间轴示意图中的2:30
	 * param: max_count   请求的时间区间数目
	 * param: count       实际返回的区间数目
	 * param: range_list  sdk动态分配的内存空间，接口实现者需要把区间数据写入其中，由sdk负责释放。一定要和实际返回的数目count匹配，否则会出现未知错误
	 *
     * 特别说明：在回调处理里面，先确认有没有sd存储卡，若不存在，请将count值赋值为 -1。我们会将没有SD存储卡这个情况，通知给手Q，做界面处理。
     */
	void (*on_fetch_history_video)(int channel, unsigned int last_time, int max_count, int *count, tx_history_video_range * range_list);

	/**
	 * 接口说明: 播放历史视频 step2
	 * param: play_time  单位是秒，历史视频开始播放的起始时间(unix时间戳)。
	 * param: base_time  单位是毫秒，主要用于填充历史视频数据时修正tx_set_video_data接口的nTimeStamps参数，具体用法是，nTimeStamps = frameTimeStamp - base_time，
     *                   frameTimeStamp 精确到毫秒。SDK1.4 可以忽略这个值。	
	 *
	 * ####################################################################
	 * ##
     * ##     play_time == 0 代表播放实时视频                	 
	 * ##
	 * ####################################################################

     * 
	 */
	void (*on_play_history_video)(unsigned int play_time, unsigned long long base_time);
} tx_history_video_notify;

/**
 * 使用历史视频功能需要在设备上线之后进行初始化，传入回调方法
 * param: notify 设备回调通知
 */
SDK_API void tx_init_history_video_notify(tx_history_video_notify *notify);


/**
 * 向SDK填充历史视频数据
 * nTimeStamps:  视频frame的时间戳，单位ms，相对于1970年1月1日。
 */
SDK_API void tx_set_history_video_data(unsigned char *pcEncData, int nEncDataLen,
        int nFrameType, unsigned long long nTimeStamps, int nGopIndex, int nFrameIndex, int nTotalIndex, int nAvgQP);


/**
 * 向SDK填充历史音频数据
 */
SDK_API void tx_set_history_audio_data(tx_audio_encode_param *param, unsigned char *pcEncData, int nEncDataLen, unsigned long long nTimeStamps);


/**
 * 设置初始的清晰度, 必须在tx_start_av_service之前调用
 * definition: 要设置的清晰度，具体取值参见 enum definition
*/
SDK_API void tx_set_init_video_definition(int definition);

/**
 * 设置【无用户观看时tx_av_callback.on_stop_camera】通知的超时时间
 * 默认无用户观察时，1分钟才会调用on_stop_camera，开发者可以修改这里的值，控制超时的时间，
 * 超时时间(ms), 最小值支持20*1000ms
*/
SDK_API void tx_set_stop_camera_timeout(int interval);

CXX_EXTERN_END

#endif // __IPCAMERA_H__
