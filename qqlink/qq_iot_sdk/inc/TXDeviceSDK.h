#ifndef __TX_DEVICE_SDK_H__
#define __TX_DEVICE_SDK_H__

#include "TXAudioVideo.h"
#include "TXMsg.h"
#include "TXDataPoint.h"
#include "TXFileTransfer.h"

CXX_EXTERN_BEGIN

enum tx_test_mode_enum
{
    test_mode_default           = 0, // 接入正式环境
    test_mode_test_env          = 1, // 登录到测试环境
};

enum tx_network_type
{
	network_type_none			= 0,
	network_type_wifi			= 1, // wifi网络以及除了xg移动、xg联通、xg电信、香港之外的网络
	network_type_mobile			= 2, // 2g/3g/4g下的移动网络
	network_type_unicom			= 3, // 2g/3g/4g下的联通网络
	network_type_telecom		= 4, // 2g/3g/4g下的电信网络
	network_type_hongkong		= 5, // 香港
};

enum tx_sdk_run_mode
{
	sdk_run_mode_default		= 0, //正常模式
	sdk_run_mode_low_power		= 1, //低功耗模式
};

enum tx_cache_binderinfo_mode 
{
	tx_cache_binderinfo_mode_auto = 0, // 自动模式。自动模式下，如果是android和windows平台则开启绑定者列表缓存，如果其他平台则不开启
	tx_cache_binderinfo_mode_on,       // 开启缓存
	tx_cache_binderinfo_mode_off,      // 禁用缓存
};

//设备信息：需要在初始化设备时提供的一些硬件相关信息，比如操作系统、ip地址等等
typedef struct _tx_device_info
{
    //操作系统信息
    char *                      os_platform;                //操作系统类型:iOS,Android,Synbian等    带'\0'结尾的字符串

    //网络信息
    int                         network_type;				//设备当前连接的网络，必须从枚举变量tx_network_type中取值
                                                            //SDK会根据不同的网络类型连接对应的ip服务器，优化性能

    //硬件信息
    char *                      device_name;                //设备名称,长度不超过32字节    带'\0'结尾的字符串
    char *                      device_serial_number;       //设备序列号,长度不超过16字节    带'\0'结尾的字符串
    char *                      device_license;             //设备LICENSE    带'\0'结尾的字符串
    int                         product_version;            //设备版本

    //即通Server分配的信息
    int                         product_id;                 //每一个厂商的每一种类型的设备对应的一个id
    char *                      server_pub_key;             //设备登陆/注册使用的加密server公钥    带'\0'结尾的字符串

    unsigned int                test_mode;                  //测试环境标志位
    int							run_mode;					//SDK运行模式，必须从枚举变量tx_sdk_run_mode中取值
    int						    support_lan_av;				//IPCamera是否支持局域网音视频功能，主要供TV在局域网获取Camera的音视频数据，默认为0表示不支持，支持此必须实现TXLanAudioVideo.h里面的接口
	int                         cache_binderinfo_mode;      //是否在本地保存绑定者信息，当设备的授权者个数很多时，开启缓存可以加快重启进程后获取设备绑定者列表的速度
} tx_device_info;

//设备绑定者信息
typedef struct tag_tx_binder_info
{
    int                 type;			//绑定者类型
    unsigned long long  tinyid;			//绑定者tinyid
    unsigned long long  uin;			//绑定者uin, 出于保护用户隐私的考虑，这个值总是0，如果确实有获取uin的需求，请联系qq物联helper
    char                nick_name[128];	//绑定者昵称
    int                 gender;			//绑定者性别
    char                head_url[1024];	//绑定者头像url
}tx_binder_info;

//设备通知：登录、在线状态、消息等相关的事件通知
typedef struct _tx_device_notify
{
	// 成功上传设备注册信息到服务器（用于跨网绑定模式下是否展示二维码的依据，有此回调则表示信息上传成功，可以展示二维码）
	void (*on_wlan_upload_register_info_success)();

    // Login complete callback
    void (*on_login_complete)(int error_code);

    // Online status changed ---- status取值为：11 在线、21 离线
    void (*on_online_status)(int old_status,  int new_status);

    // binder list change callback
    void (*on_binder_list_change)(int error_code, tx_binder_info * pBinderList, int nCount);

    //手Q端或者后台远程重启设备
    void (*on_device_reboot)();
} tx_device_notify;


//SDK初始化目录：SDK会在这些目录下写文件，厂商需要同时指定每个目录下可写存储空间的大小(单位：字节)，SDK会保证不把设备写爆.
//这三个目录中的system_path必须提供；app_path尽量提供，以方便追查问题；temp_path建议提供
//这三个目录取值可以相同，如果相同，需要注意合理分配每个目录可写存储空间的大小
typedef struct _tx_init_path
{
    //SDK会在该目录下写入保证正常运行必需的配置信息；
    //SDK对该目录的存储空间要求（最小大小：10K，建议大小：100K），SDK写入次数较少，读取次数较多
    char *                  system_path;
    unsigned int            system_path_capicity;

    //SDK会在该目录下写入运行过程中的异常错误信息
    //SDK对该目录的存储空间要求较大（最小大小：300K，建议大小：1M），SDK写入次数较多，读取次数较少
    char *                  app_path;
    unsigned int            app_path_capicity;

    //SDK会在该目录下写入临时文件
    char *                  temp_path;
    unsigned int            temp_path_capicity;
} tx_init_path;


//设备绑定者类型
enum tx_binder_type
{
	binder_type_unknown         = 0,
	binder_type_owner           = 1, //主人
	binder_type_sharer          = 2, //共享者
};

//设备绑定者性别
enum tx_binder_gender
{
	binder_gender_unknown       = -1,
	binder_gender_male          = 0, //男
	binder_gender_female        = 1, //女
};


/**
* 接口说明：初始化设备SDK
*/
SDK_API int tx_init_device(tx_device_info *info, tx_device_notify *notify, tx_init_path* init_path);

/**
*
*/
SDK_API int tx_ack_app(unsigned int ip, unsigned int port);

/**
* 接口说明：退出所有设备SDK相关逻辑
*/
SDK_API int tx_exit_device();


/**
 * 接口说明:获取SDK版本号
 * 参数说明: main_ver     :  主版本号
 *          sub_ver      :  子版本号
 *          build_no     :  编译代号
 *
 */
SDK_API int tx_get_sdk_version(unsigned int *main_ver, unsigned int *sub_ver, unsigned int *build_no);


/**
 * 接口说明:写log的回调
 * 回调函数参数说明：
 *         level   log级别 取值有 0 严重错误；1 错误；2 警告；3 提示；4 调试
 *         module  模块
 *         line    行号
 *         message log内容
 */
typedef void (*tx_log_func)(int level, const char* module, int line, const char* message);

/**
 * 接口说明:用于设置写log的回调
 * 回调函数参数说明：
 *          log_func   回调函数
 *          log_write  是否写log到文件，0代表不写，1代表写；SDK可以将该log文件上传到物联后台，方便问题追踪, 所以建议填1。
 *          log_debug_print  是否允许SDK向写log的回调函数传递 调试级别的log ， 0代表不传，1代表传。
 */
SDK_API void tx_set_log_func(tx_log_func log_func, int log_write, int log_debug_print);

/**
* 接口说明：上传sdk log，使用场景，有用户交互的场景。
* 增加该接口目的是，将sdk运行的error级别的log上传到服务器，用于问题的分析和定位。用户触发设备设备上传。
* 如果不是这样的场景，建议不要调用该接口。
*
*/
SDK_API void tx_upload_sdk_log();


/**
* 接口说明：获取设备绑定者列表。 
* WARN: system_path目录如果被清除，则不能拉取到绑定者列表
*/
typedef void (*on_get_binder_list_result)(tx_binder_info * pBinderList, int nCount);
SDK_API int tx_get_binder_list(tx_binder_info * pBinderList, int* nCount, on_get_binder_list_result callback);


/**
* 接口说明：获取设备DIN
*/
SDK_API unsigned long long tx_get_self_din();


/**
 * 接口说明：解绑设备，所有的绑定关系将解除，设备将处于未绑定，未登陆状态
 */
typedef void (*on_erase_all_binders)(int error_code);
SDK_API int tx_erase_all_binders(on_erase_all_binders callback);


/**
 * 接口说明: 获取腾讯云服务器标准校时时间
 * retvalue: 返回是32位服务器校时时间，内部使用monotonic时间作为累加器，避免受合作方原校时逻辑的影响
 *           如果没有登录成功，则此接口只返回 0
 */
SDK_API int tx_get_server_time();




CXX_EXTERN_END

#endif // __TX_DEVICE_SDK_H__
