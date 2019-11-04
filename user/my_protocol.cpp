#include "my_protocol.h"



//构造方法
My_Protocol::My_Protocol()
{

}

//析构方法
My_Protocol::~My_Protocol()
{

}


unsigned short My_Protocol::get_crc(unsigned char *ptr,unsigned short len)
{
    unsigned char i;
    unsigned short crc = 0;

    for(i=len;i>0;i--)
    {
        crc = *ptr + crc;
        ptr++;
    }

    return crc;
}



bool My_Protocol::check_crc(TYP_UN_COMMUNICATE_BUFF *p_data)
{
    unsigned short crc,crc1;
    unsigned char len;
    TYP_UN_COMMUNICATE_BUFF *p_data_buff = 0;

    p_data_buff = (TYP_UN_COMMUNICATE_BUFF *)p_data;

    if(p_data_buff->Frame.head != 0xF5)return false;

    len = p_data_buff->Frame.len;

    crc = get_crc((unsigned char*)&p_data_buff->Frame.cmd,len);

    crc1 = ((p_data_buff->Frame.crc_hi << 8) | p_data_buff->Frame.crc_lo);

    if(crc1 != crc) return false;

    return true;
}


unsigned char My_Protocol::protocol_data_package(unsigned char cmd,unsigned char len,unsigned char *p_in,TYP_UN_COMMUNICATE_BUFF *p_data)
{
    unsigned short crc;

    p_data->Frame.head = FRAME_HEAD;
    p_data->Frame.cmd = cmd;
    p_data->Frame.len = len+1;

    if((p_in != 0) && (len > 0))
        memcpy(p_data->Frame.data, p_in, len);

    crc = get_crc(&p_data->Frame.cmd,p_data->Frame.len);

    p_data->Frame.crc_lo = (unsigned char)crc;
    p_data->Frame.crc_hi = (unsigned char)(crc >> 8);

    return (len+FRAME_HEAD_LEN);
}


unsigned char My_Protocol::protocol_data_unpackage(TYP_UN_COMMUNICATE_BUFF *p_in,unsigned char *p_out)
{
    if(check_crc(p_in))
    {
        memcpy(p_out,&p_in->Frame.cmd,p_in->Frame.len);

        return (p_in->Frame.len);
    }
    else return 0;
}



