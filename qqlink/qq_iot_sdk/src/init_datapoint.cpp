#include "qqlink/qq_iot_sdk/inc/init_datapoint.h"


using namespace std;

//构造方法
Init_DataPoint::Init_DataPoint()
{
}

//析构方法
Init_DataPoint::~Init_DataPoint()
{

}



// 处理设备关心的控制端指令
void Init_DataPoint::on_receive_datapoint(unsigned long long from_id, tx_data_point * data_points, int data_points_count)
{
    Init_DataPoint data_point_handle;

    /*步骤1：先打印出来看看*/
    int i = 0;
    printf("=================================\n");
    printf("on_receive_data_point\n");
    printf("=================================\n");

    while (i < data_points_count)
    {
        printf("data_points[i].id = %u \n",data_points[i].id);

        if (data_points[i].id == LED1_ID)      // LED1_ID = 100002892  ,
        {
            data_point_handle.set_led_state( from_id, data_points[i] );
        }

        if(data_points[i].id== LED2_ID)   // LED2_ID = 100003251
        {
            data_point_handle.set_led_state( from_id, data_points[i] );
        }

        if (data_points[i].id == BRIGHTNESS_ID)                  //100002895
        {
                //on_receive_led_light(from_id, data_points[i]);
            data_point_handle.brightness_process( from_id, data_points[i] );
        }

        if (data_points[i].id == TEMPER_SWITCH_ID)            //100003271
        {
                //on_receive_temper_switch(from_id, data_points[i]);
            data_point_handle.update_temp_to_qq( from_id, data_points[i] );
        }

        ++i;
    }
}

//ret的回调函数
void Init_DataPoint::ret_call_msg(unsigned int cookie, int err_code)
{
        (void)cookie;
        (void)err_code;

#if 0
        printf("==============================\n");
        printf("***ret_call_msg: cookie[%d] err_code[%d] \n", cookie, err_code);
        printf("==============================\n");
#endif
}

//报告led的状态
void Init_DataPoint::report_led_status(unsigned int cookie, int err_code)
{
        printf("==============================\n");
        printf("***report_led_status: cookie[%d] err_code[%d] \n", cookie, err_code);
        printf("==============================\n");
}

//回复led的状态
void Init_DataPoint::ask_led_status(unsigned int cookie, unsigned long long from_client, int err_code)
{
        printf("==============================\n");
        printf("***ask_led_status: cookie[%d] from_client[%lld] err_code[%d] \n", cookie, from_client,err_code);
        printf("==============================\n");
}

//回复亮度
void Init_DataPoint::ret_brightness(unsigned int cookie, int err_code)
{
        printf("==============================\n");
        printf("***ret_led_light: cookie[%d] err_code[%d] \n", cookie, err_code);
        printf("==============================\n");
}


//向手机qq更新温度数据
int Init_DataPoint::update_temp_to_qq(unsigned long long from_id, tx_data_point  datapoint)
{
    (void)from_id;

    if(strncmp(datapoint.value, "1", 1) == 0)
    {
        printf("turn on recycle temper report \n");
        data_point_send.update_flag = 1;
    }
    else if(strncmp(datapoint.value, "0", 1) == 0)
    {
        printf("turn off recycle temper report \n");
        data_point_send.update_flag = 0;
    }
    else{}

    //emit this->send_data_point(data_point_send);

    return true;
}

//设置led 状态
void Init_DataPoint::set_led_state(unsigned long long from_id, tx_data_point  datapoint)
{
        (void)from_id;
#if 0
        Led_Control led_ctrl;
        unsigned char led_num;
        unsigned char led_state;
        unsigned int ret_code;

        if(datapoint.id == LED1_ID)
        {
                led_num = LED_NUM1;
        }
        else if(datapoint.id == LED2_ID)
        {
                led_num = LED_NUM2;
        }
        else{}

        if(strncmp(datapoint.value, "1", 1) == 0)
        {
                ret_code = 1;
                led_state = LED_ON;
        }
        else if(strncmp(datapoint.value, "0", 1) == 0)
        {
                ret_code = 0;
                led_state = LED_OFF;
        }
        else{};

        switch(led_num)
        {
                case LED_NUM1:
                    if(led_state == LED_ON)data_point_send.led1_state = 1;
                    else data_point_send.led1_state = 0;
                break;

                case LED_NUM2:
                    if(led_state == LED_ON)data_point_send.led2_state = 1;
                    else data_point_send.led2_state = 0;
                break;

                default:  break;
        }

        led_ctrl.led_state_ctrl(led_num,led_state);
#endif

    //emit this->send_data_point(data_point_send);

    //指令处理结果回传到手机(注意seq一定要配对，不要随便乱填哦)
    tx_report_data_point(datapoint.id, datapoint.value, NULL, report_led_status);

    //也可以使用tx_ack_data_point，但是参数太多，容易出错
    //tx_ack_data_point(from_id, datapoint.id, datapoint.value, datapoint.seq,ret_code, NULL, ask_led_status);
}

//灯光亮度控制
void Init_DataPoint::brightness_process(unsigned long long from_id, tx_data_point  datapoint)
{
    (void)from_id;

    //解析json字符串(此处省略冗长的解析代码)
    char* json = datapoint.value;
    char *ret_value=datapoint.value;
    //int ret =0;
    int count=0;
    char number[4]={0};
    int  light=0;
    //int  i=0;

    printf("=================================\n");
    printf("=======on_receive_led_status\n");
    printf("=================================\n");
    printf("***json=%s\n", json);


    while(*json != '\0')
    {
        if(*json>='0' && *json<='9')
        {
            number[count] = *json;    //将数字存放到数组number中
            #if  1
            light *= 10;
            light += number[count]-'0';
            #endif
            count++;
        }
        json++;    //指向下一个字符
    }
    printf("***light=%d\n", light);

    data_point_send.brightness = light;

    tx_report_data_point(BRIGHTNESS_ID, ret_value, NULL, ret_brightness);             //100002895
}

//初始化数据点
int Init_DataPoint::init_data_point()
{
    //int ret = -1;

    //挂接datapoint的处理函数
    tx_data_point_notify dataPointNotify;
    bzero(&dataPointNotify,sizeof(tx_data_point_notify));

    dataPointNotify.on_receive_data_point = on_receive_datapoint;

    // 初始化datapoint
    tx_init_data_point(&dataPointNotify);

    return true;
}
