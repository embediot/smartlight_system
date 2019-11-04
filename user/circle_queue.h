#ifndef __CIRCLE_QUEUE_H_
#define __CIRCLE_QUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QMutex>

class Circle_queue
{
#define   MAX_CIRCLE_QUEUE_BUFF_LEN            10240

public:
    Circle_queue();
    ~Circle_queue();
    unsigned int circlebuff_read_packet_data(char *p_data);
    unsigned int circlebuff_write_packet_data(char *s, unsigned int len);

private:
    unsigned int    read_pos;
    unsigned int    write_pos;
    char   buff[MAX_CIRCLE_QUEUE_BUFF_LEN];

    QMutex queue_mutex;

    void circle_queue_init(void);
    bool is_circlebuff_have_data(void);
    bool circlebuff_write_one_byte(char byte);
    char circlebuff_read_one_byte(void);
};




#endif
