#include "qqlink/qq_iot_sdk/inc/init_device.h"

using namespace std;

//构造方法
Init_Device::Init_Device()
{

}

//析构方法
Init_Device::~Init_Device()
{

}

// 标记是否已经启动音视频服务
//static bool g_start_av_service = false;

/**
 * 登录完成的通知，errcode为0表示登录成功，其余请参考全局的错误码表
 */
void Init_Device::on_login_complete(int errcode)
{
    printf("on_login_complete | code[%d]\n", errcode);
}

/**
 * 在线状态变化通知， 状态（status）取值为 11 表示 在线， 取值为 21 表示  离线
 * old是前一个状态，new是变化后的状态（当前）
 */
void Init_Device::on_online_status(int old_status, int new_status)
{
    //int err =0;

    printf("old status: %d\n",old_status);
    printf("new status: %d\n",new_status);
    printf("online status: %s\n", 11 == new_status ? "true" : "false");

    if(21 == new_status)
    {
        printf(" //====off line======\n");
    }

    // 上线成功，启动音视频服务
    //if(11 == new_status && !g_start_av_service) {
    if(11 == new_status )
    {
        printf(" //====on line======\n");
    }
}

/**
 * 辅助函数：SDK的log输出回调
 * SDK内部调用改log输出函数，有助于开发者调试程序
 */
void Init_Device::log_func(int level, const char* module, int line, const char* message)
{
    (void) level;
    (void) module;
    (void) line;
    printf("%s\n", message);
}



/**
 * 辅助函数: 从文件读取buffer
 * 这里用于读取 license 和 guid
 * 这样做的好处是不用频繁修改代码就可以更新license和guid
 */
bool Init_Device::read_buffer_from_file(char *file_path, unsigned char *p_buff, int in_size, int *size_used)
{
    if (!file_path || !p_buff) return false;

    int len = 0;
    FILE * file = fopen(file_path, "rb");
    if (!file) return false;

    fseek(file, 0L, SEEK_END);
    len = ftell(file);
    fseek(file, 0L, SEEK_SET);

    if ((0 == len) || (in_size < len))
    {
        printf("invalide file or buffer size is too small...\n");
        fclose(file);
        file = NULL;
        return false;
    }

    *size_used = fread(p_buff, 1, len, file);
    // bugfix: 0x0a is a lineend char, no use.
    if (p_buff[len-1] == 0x0a)
    {
        *size_used = len - 1;
        p_buff[len-1] = '\0';
    }

    printf("len:%d, len:%d\n",len, *size_used);
    fclose(file);
    file = NULL;

    return true;
}

void Init_Device::log_on()
{
    tx_set_log_func(log_func,1,1);
}

void Init_Device::log_off()
{
    tx_set_log_func(NULL,0,0);
}

//判断文件是否可被访问
int Init_Device::file_is_exist(char *file_path)
{
    int  ret = -1;

    ret = access(file_path, 0);

    return  ret;
}

 //设备信息初始化
int Init_Device::dev_config_init(void)
{
    FILE    *fp = NULL;
    char 	*line = NULL;
    size_t  len = 0;
    ssize_t read = 0;
    char    buf[128] = {0};

    //1.检查配置文件是否存在，不存在则退出
    if( file_is_exist((char*)"/opt/smartlight/qq_iot/demo_bind/config")  != 0)
    {
        printf("[error]open config failed...\n");
        return -1;
    }

    //1.打开配置文件
    fp = fopen("/opt/smartlight/qq_iot/demo_bind/config", "rb");
    if (!fp) return -1;

    //2.读取第一行的数据
    read = getline(&line, &len, fp);
    memset(buf,0,128);//将buf清空
    strncpy(buf,line,read - 1);
    buf[read] = '\0';               //加上字符串结束符
    product_id = atoi(buf);
    //printf("Product_id = %d\n",product_id);


    //3.读取第二行的数据
    read = getline(&line, &len, fp);
    memset(buf,0,128);                                //将buf清空
    strncpy(buf,line,read - 1);
    buf[read] = '\0';                    //加上字符串结束符
    sprintf(pubkey_file, "/opt/smartlight/qq_iot/demo_bind/%s",buf);
    //printf("Pubkey_file = %s\n",pubkey_file);

    //检查该文件是否存在
    if( file_is_exist(pubkey_file) )
    {
        printf("[error]access %sfailed...\n",pubkey_file);
        return -1;
    }


    //4.读取第三行的数据
    read = getline(&line, &len, fp);
    memset(buf,0,128);        //将buf清空
    strncpy(buf,line,read - 1);
    buf[read] = '\0';//加上字符串结束符
    sprintf(guid_file, "/opt/smartlight/qq_iot/demo_bind/%s",buf);
    //printf("GUID_file = %s\n",guid_file);

    //检查该文件是否存在
    if( file_is_exist(guid_file) )
    {
        printf("[error]access %sfailed...\n",guid_file);
        return -1;
    }

    //5.读取第四行的数据
    read = getline(&line, &len, fp);
    memset(buf,0,128);//将buf清空
    strncpy(buf, line, read - 1);
    buf[read] = '\0';                 //加上字符串结束符
    sprintf(licence_file, "/opt/smartlight/qq_iot/demo_bind/%s",buf);
    //printf("Licence_file = %s\n",licence_file);

    //检查该文件是否存在
    if( file_is_exist(licence_file) )
    {
        printf("[error]access %sfailed...\n",licence_file);
        return -1;
    }

   if (line)
   {
        free(line);
        line = NULL;
   }

    if(fp)
    {
        fclose(fp);
        fp = NULL;
    }

    return 0;

}


/**
 * SDK初始化
 * 例如：
 * （1）填写设备基本信息
 * （2）打算监听哪些事件，事件监听的原理实际上就是设置各类消息的回调函数，
 * 	例如设置Datapoint消息通知回调：
 * 	开发者应该定义如下的 my_on_receive_data_point 函数，将其赋值tx_data_point_notify对象中对应的函数指针，并初始化：
 *
 * 			tx_data_point_notify msgOnRecv= {0};
 * 			msgOnRecv.on_receive_data_point = my_on_receive_data_point;
 * 			tx_init_data_point(&msgOnRecv);
 *
 * 	那么当SDK内部的一个线程收到对方发过来的DataPoint消息后（通过服务器转发），将同步调用 msgOnRecv.on_receive_data_point
 */
int Init_Device::init_qq_device()
{
    if( dev_config_init() < 0)return -1;

    printf("Product_id = %d\n",product_id);
    printf("Pubkey_file = %s\n",pubkey_file);
    printf("GUID_file = %s\n",guid_file);
    printf("Licence_file = %s\n",licence_file);

    // 读取 license
    char license[256];
    int license_size = 0;
    bzero(license,sizeof(license));

    //if (!read_buffer_from_file("./licence.sign.file.txt", license, sizeof(license), &license_size)) {
    if (!read_buffer_from_file(licence_file, (unsigned char*)license, sizeof(license), &license_size))
    {
        printf("[error]get license from file failed...\n");
        return -1;
    }

    // 读取guid
    char guid[32];
    int guid_size = 0;
    bzero(guid,sizeof(guid));

    //if(!read_buffer_from_file("./guid_file.txt", guid, sizeof(guid), &guid_size)) {
    if(!read_buffer_from_file(guid_file, (unsigned char*)guid, sizeof(guid), &guid_size))
    {
        printf("[error]get guid from file failed...\n");
        return -1;
    }

    char svr_pubkey[256];
    int pubkey_size = 0;
    bzero(svr_pubkey,sizeof(svr_pubkey));

    //if (!read_buffer_from_file("./1000000004.pem", svr_pubkey, sizeof(svr_pubkey), &pubkey_size))
    if (!read_buffer_from_file(pubkey_file, (unsigned char*)svr_pubkey, sizeof(svr_pubkey), &pubkey_size))
    {
        printf("[error]get svr_pubkey from file failed...\n");
        return -1;
    }

    // 设备的基本信息
    tx_device_info info;
    bzero(&info,sizeof(tx_device_info));

    info.os_platform            = (char*)"Linux";
    info.device_name            = (char*)"demo1";
    info.device_serial_number   = guid;
    info.device_license         = license;
    info.product_version        = 1;
    info.network_type			= network_type_wifi;
    info.product_id             =  product_id;
    info.server_pub_key         = svr_pubkey;

    // 设备登录、在线状态、消息等相关的事件通知
    // 注意事项：
    // 如下的这些notify回调函数，都是来自硬件SDK内部的一个线程，所以在这些回调函数内部的代码一定要注意线程安全问题
    // 比如在on_login_complete操作某个全局变量时，一定要考虑是不是您自己的线程也有可能操作这个变量
    tx_device_notify notify;
    bzero(&notify,sizeof(tx_device_notify));

    notify.on_login_complete     = on_login_complete;
    notify.on_online_status      = on_online_status;
    notify.on_binder_list_change = NULL;

    // SDK初始化目录，写入配置、Log输出等信息
    // 为了了解设备的运行状况，存在上传异常错误日志 到 服务器的必要
    // system_path：SDK会在该目录下写入保证正常运行必需的配置信息
    // system_path_capicity：是允许SDK在该目录下最多写入多少字节的数据（最小大小：10K，建议大小：100K）
    // app_path：用于保存运行中产生的log或者crash堆栈
    // app_path_capicity：同上，（最小大小：300K，建议大小：1M）
    // temp_path：可能会在该目录下写入临时文件
    // temp_path_capicity：这个参数实际没有用的，可以忽略
    tx_init_path init_path;
    bzero(&init_path,sizeof(tx_init_path));

    init_path.system_path = (char*)"./";
    init_path.system_path_capicity = 100 * 1024;
    init_path.app_path = (char*)"./";
    init_path.app_path_capicity = 1024 * 1024;
    init_path.temp_path = (char*)"./";
    init_path.temp_path_capicity = 10 * 1024;

    // 设置log输出函数，如果不想打印log，则无需设置。
    // 建议开发在开发调试阶段开启log，在产品发布的时候禁用log。
    log_on();

    // 初始化SDK，若初始化成功，则内部会启动一个线程去执行相关逻辑，该线程会持续运行，直到收到 exit 调用
    int ret = tx_init_device(&info, &notify, &init_path);
    if (err_null == ret)
    {
        printf(" >>> tx_init_device success\n");
    }
    else
    {
        printf(" >>> tx_init_device failed [%d]\n", ret);
        return -1;
    }

    return product_id;

}

//退出所有设备sdk相关逻辑
int Init_Device::exit_qq_device()
{
    int ret = -1;

    ret = tx_exit_device();

    return ret;
}
