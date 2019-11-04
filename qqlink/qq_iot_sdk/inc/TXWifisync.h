#ifndef __TX_WIFI_SYNC_H__
#define __TX_WIFI_SYNC_H__

#include "TXSDKCommonDef.h"

CXX_EXTERN_BEGIN

/*
 *                      SmartLink API简介
 * SmartLink API有两套接口，目前推荐使用2.0接口，2.0接口简单，1.0接口使用复杂不推荐使用；
 * 三部分构成：common API：公共部分，1.0、2.0接口均使用；
             2.0接口API：2.0接口部分；
             1.0接口API：1.0接口部分；
 */

////////////////////////////////////////////////////////////////////////////////////////
//     
//        			    			common API
//
////////////////////////////////////////////////////////////////////////////////////////

/** init_wifi_sync & start_smartlink的返回值使用此enum定义
 */
enum wifisyncerror {
	QLERROR_INIT_SUCCESS        = 0,         //suc
	QLERROR_MEMORY_ALLOC  		= 1,        //内存分配失败
	QLERROR_NOTIFY_NULL		  	= 2,        //FUNC_NOTIFY回调函数为空
	QLERROR_PARAM_KEY       	= 3,        //key传入为NULL
	QLERROR_PARAM_KEY_LEN   	= 4,        //key长度不合法
	QLERROR_OPENSSL_LOAD_FAILED = 5,        //加载openssl库失败
	QLERROR_HOP_NULL			= 6,		//跳频回调函数为空
	QLERROR_SZIFNAME_INVALID	= 7,		//szifname不合法
	QLERROR_WIFICHIP_NOTSUPPORT = 8,		//此WIFI CHIP 2.0接口不支持
	QLERROR_INIT_OTHER		    = 9,		//其他错误
};

/** fill_80211_frame的返回值使用此enum定义
 */
enum fill80211relust {
	QLERROR_SUCCESS 		= 0,		//解析成功
	QLERROR_HOP				= 1,		//收到此返回值表示当前信道锁定有误，立马切换下一个信道
	QLERROR_LOCK    		= 2,        //收到此返回值表示是当前信道，锁定

	//下面这些返回值供调试使用，切信道，锁信道根据上面三个值确定,BCAST表示Braodcast，MCAST表示Multicast
	QLERROR_OTHER			= 3,    //其它错误
	QLERROR_DECRYPT_FAILED  = 4,    //解密出错
	QLERROR_NEED_INIT     	= 5,    //wifi sync没有初始化
	QLERROR_VERSION        	= 6,    //SDK版本不对应
	QLERROR_START_FRAME     = 7, 	//无效的包

    //Broadcast relative
    QLERROR_BCAST_NOT_FRAME	= 8, 	//不是广播包
    QLERROR_BCAST_CALC_C	= 9,	//成功计算C值
    QLERROR_BCAST_ONE_DATA	= 10,	//解析到一个广播数据

    //Multicast relative
    QLERROR_MCAST_NOT_FRAME	= 11,  //不是组播包
    QLERROR_MCAST_ONE_DATA	= 12,  //解析到一个组播数据
};

#define QLMAX_SSID_LEN      128
#define QLMAX_PSWD_LEN      128
#define QLMAX_IP_LEN        16

typedef struct
{
	char                sz_ssid[QLMAX_SSID_LEN];      // AP账号名称
	char                sz_password[QLMAX_PSWD_LEN];  // AP密码
    char                sz_ip[QLMAX_IP_LEN];          // 发送端IP地址，设备连接AP后反馈给发送端ack时用到
    unsigned short      sh_port;                      // 发送端端口，  设备连接AP后反馈给发送端ack时用到
    unsigned char       sz_bssid[6];                  // AP的48位MAC地址， 主要用于连接隐藏SSID广播的AP，厂商可根据实际情况选择是否使用
	                                                  // 如果设备可以扫描到beacon帧并通过beancon中的信息连接AP则可忽略此成员
} tx_wifi_sync_param;


/**  跳頻，1~13,根据传入的信道值跳到指定的信道，需要厂商实现跳频功能
 @param nchannel  要跳转的信道值
 */
typedef void (* FUNC_HOP)(int nchannel);


/**  smartlink完成后回调，需要厂商自己实现，主要是返回连接路由器需要的SSID与PWD
 @param pqq_link_param  包含路由器SSID和密码信息，回调结束后会被销毁
 @param puserdata  厂商传入的数据指针
 */
typedef void (* FUNC_NOTIFY)(tx_wifi_sync_param *pwifi_sync_param, void *puserdata);





///////////////////////////////////////////////////////////////////////////////////////////
//     
//        					SmartLink2.0 API   推荐使用
//
///////////////////////////////////////////////////////////////////////////////////////////

/*
 *                                    SmartLink2.0接口使用说明
 * step1. 请确认使用的芯片是否包含在Enum WifiChipType里面，暂不区分小型号，如果不包含请直接填写WCT_80211RADIOTAP_STANDARD尝试下，看能否成功，如果成功就使用2.0接口，如果不成功请使用1.0接口或者联系helper
 * step2. 调用start_smartlink接口，传入相关参数，具体请参见函数说明；
 * step3. 解析成功回调FUNC_NOTIFY，拿到SSID及PWD，厂商此时连接AP；
 * step4. 只有在需要强制停止smartlink时，才调用stop_smartlink函数，如声波配网成功后，一般无需调用；
 */

/**
 * 以下WifiChipType里面没有的芯片，请直接填写WCT_80211RADIOTAP_STANDARD尝试下，看能否成功，如果不成功请使用1.0接口或者联系helper
 */
typedef enum {
	WCT_REALTEK8188,
	WCT_MTK7601,
    WCT_80211RADIOTAP_STANDARD,//表示芯片采用标准的radiotap head
} WifiChipType;


/**  start_smartlink，会起一个线程->抓包->解析->回调通知->结束线程
 @param fHopping  参数必填，跳频回调函数
 @param szifName  参数必填，网卡名称，常见的是 wlan0
 @param fNotify   参数必填，完成后回调通知函数
 @param szSN  参数必填，16字节的SN,确保和二维码 http://iot.qq.com/add?pid=XXXX&sn=XXXX 中的sn内容一致
 @param chipType  参数必填，厂商芯片类型，参见WifiChipType
 @param hoppingTime  参数可选，默认填0，对应默认值200 ms，跳频时间间隔，不要低于200ms
 @param snifferTime  参数可选，默认填0，对应默认值10000 ms，锁定信道时间，不要低于10000ms
 @param pUserData  参数可选，默认填NULL，用户上下文数据，Smartlink不关心其内容，在回调中原样传回
 @return  QLERROR_INIT_SUCCESS表示成功，其他错误码请参见wifisyncerror定义
*/
SDK_API int start_smartlink(
    FUNC_HOP fHopping,
    char* szifName,
    FUNC_NOTIFY fNotify,
    char* szSN, 
	WifiChipType chipType,
    int hoppingTime,
    int snifferTime,
    void* pUserData
    );


/**  调用此API来停止smartlink，一般不需要调用此函数，只有在需要强制停止smartlink时，才需要调用stop_smartlink函数，如声波配网成功后
 @note
     一定不能在FUNC_NOTIFY回调里面调用此函数
 */
SDK_API void stop_smartlink();


/**  is_smartlink_running
*/
SDK_API int is_smartlink_running();





////////////////////////////////////////////////////////////////////////////////////////////////
//
//              SmartLink1.0 API   除非2.0接口不满足需求否则不推荐使用
//
////////////////////////////////////////////////////////////////////////////////////////////////

/*                             SmartLink1.0接口使用说明
 * 由于不知道wifisync 发送端在那个channel上发包，因此需要在每个可能的channel上抓包，分析，重复这个过程，直到回调通知。
 *
 * step1. init_wifi_sync初始化smartlink
 * step2. 开启监听模式，并将信道切到下一个信道n（1~13），同时调用wifi_sync_notify_hop(n), 并在当前信道停留200ms
 * step3. 抓包调用fill_80211_frame函数
 * 	 step2.1 当fill_80211_frame返回QLERROR_LOCK时，检查出参(*pChannel)与当前信道是否一致，不一致则调整到信道(*pChannel)，同时锁定当前信道停留10000ms，转step3
 *   step2.2 当fill_80211_frame返回QLERROR_HOP时，转step2
 *   step2.3 当fill_80211_frame返回QLERROR_SUCCESS时，内部回调用FUNC_NOTIFY函数，返回ssid与pwd用于连接AP，转step4
 * step4. 反初始化调用destory_wifi_sync
 */
 
/**  初始化 wifi sync
 @param fNotify  wifi sync 完成后回调通知函数
 @param szSN  16位的设备SN
 @param puserdata  保存用户数据,由调用方传入,FUNC_NOTIFY回调函数的时候会传出去
 @return  QLERROR_INIT_SUCCESS表示成功，其他错误码请参见wifisyncerror定义
 */
SDK_API int init_wifi_sync(FUNC_NOTIFY fNotify, char *szSN, void *puserdata);


/**  该函数自动判断并收集数据，当判断收集数据结束时会调用FUNC_NOTIFY回调函数，将SSID与PWD传出
 @note
     *pChannel为出参，仅在fill_80211_frame返回值为QLERROR_LOCK时，且其值为非0时有效
 @param buff  代表网卡抓到的802.11数据包
 @param nlen  代表传进来的数据包长度
 @param npackoffset  代表MPDU偏移位置，即macframe起始位置，根据芯片不同而不同具体请咨询wifi芯片原厂
 @param pChannel[out]  出参，0表示无效值，1~13表示内部评估的正确信道，仅在fill_80211_frame返回值为QLERROR_LOCK时有效
				       当fill_80211_frame返回值为QLERROR_LOCK, 且*pChannel返回值为非0时，表示内部评估出的正确信道，如果跟当前信道不一样，请调整到*pChannel信道上，同时锁定
  				       当fill_80211_frame返回值不是QLERROR_LOCK时，请忽略*pChannel的返回值
 @return  QLERROR_SUCCESS代表解析成功
          QLERROR_HOP代表需要立即切换到下一个信道
          QLERROR_LOCK代表可以锁信道
          其它返回值仅供调试使用
 */
SDK_API int fill_80211_frame(const unsigned char *buff, int nlen, int npackoffset, int* pChannel);


/**  外部切换信道时通知给sdk内部，做一些清理工作
 @note
     在fill_80211_frame返回QLERROR_LOCK时调整到正确的信道这种情况下就不要调用此函数
 @param channel  当前切换的信道值
 */
SDK_API void wifi_sync_notify_hop(int channel);


/**  反初始化 wifi sync
 */
SDK_API void destory_wifi_sync();



CXX_EXTERN_END

#endif // __TX_WIFI_SYNC_H__
