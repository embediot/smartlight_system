#include "circle_queue.h"

//构造方法
Circle_queue::Circle_queue()
{
    circle_queue_init();
}

//析构方法
Circle_queue::~Circle_queue()
{

}


void Circle_queue::circle_queue_init(void)
{
    this->read_pos = 0;
    this->write_pos = 0;
    bzero(this->buff,MAX_CIRCLE_QUEUE_BUFF_LEN);
}

bool Circle_queue::is_circlebuff_have_data(void)
{
    if (this->read_pos != this->write_pos)return true;
    else return false;
}

bool Circle_queue::circlebuff_write_one_byte(char byte)
{
    unsigned int  next_pos;

    next_pos = (this->write_pos + 1) % MAX_CIRCLE_QUEUE_BUFF_LEN;

    if ( next_pos != this->read_pos )
    {
        this->buff[this->write_pos] = byte;
        this->write_pos = next_pos;
        return true;
    }
    else return false;
}

char Circle_queue::circlebuff_read_one_byte(void)
{
    char data = 0;

    if (is_circlebuff_have_data() == true)
    {
        data = this->buff[this->read_pos];
        this->read_pos = (this->read_pos + 1) % MAX_CIRCLE_QUEUE_BUFF_LEN;
    }
    return data;
}





unsigned int Circle_queue::circlebuff_read_packet_data(char *p_data)
{
    unsigned int i = 0;
    unsigned int datalen = 0;
    unsigned char temp[2];

    queue_mutex.lock();
    if(is_circlebuff_have_data() == true)
    {
        temp[0] = (unsigned char)circlebuff_read_one_byte();
        temp[1] = (unsigned char)circlebuff_read_one_byte();

        datalen = (unsigned int)((temp[0] << 8) | temp[1]);

        for(i = 0;i < datalen;i++)
        {
            p_data[i] = circlebuff_read_one_byte();
        }
    }
    queue_mutex.unlock();

    return datalen;
}

unsigned int Circle_queue::circlebuff_write_packet_data(char *s, unsigned int len)
{
    unsigned int i = 0;
    unsigned char data_len[2];

    data_len[0] = (unsigned char)(len >> 8);
    data_len[1] = (unsigned char)len;

    queue_mutex.lock();
    for(i=0;i<2;i++)    //写入长度
    {
        circlebuff_write_one_byte((char)data_len[i]);
    }

    for (i=0; i<len; i++)
    {
        if(!circlebuff_write_one_byte(s[i]))break;
    }
    queue_mutex.unlock();

    return i;
}
