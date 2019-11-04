#include "network/network_widget.h"

Network_Widget::Network_Widget(WelcomeWidget *widget)
{
    this->welcomewidget = widget;
    this->welcome_ui = this->welcomewidget->ui;



    connect(welcome_ui->pb_register,SIGNAL(clicked()),this,SLOT(on_pb_register_clicked()));     //建立信号槽
    connect(welcome_ui->pb_clear_tx,SIGNAL(clicked()),this,SLOT(on_pb_clear_tx_clicked()));     //建立信号槽
    connect(welcome_ui->pb_clear_rx,SIGNAL(clicked()),this,SLOT(on_pb_clear_rx_clicked()));     //建立信号槽
}

Network_Widget::~Network_Widget()
{
    this->welcomewidget = NULL;
    this->welcome_ui = NULL;

}


void Network_Widget::display_serial_port_data(char *p_data,int len,char flag)
{
    QByteArray data(p_data,len);
    QString str_display = data.toHex();

    if(flag == SERIAL_PORT_RX)
    {
        welcome_ui->plainTextEdit_uart_rx->appendPlainText(str_display);
    }
    else if(flag == SERIAL_PORT_TX)
    {
        welcome_ui->plainTextEdit_uart_tx->appendPlainText(str_display);
    }
    else
    {

    }

}


void Network_Widget::control_relay(unsigned char door_num,unsigned char ctrl_flag)
{
    unsigned char data_buff[4];
    unsigned char data_len = 0;
    TYP_UN_COMMUNICATE_BUFF p_data_send;
    My_Protocol my_protocol;

    data_buff[0] = 0;
    data_buff[1] = 0;
    data_buff[2] = door_num;
    data_buff[3] = ctrl_flag;

    data_len = my_protocol.protocol_data_package(CMD_REPORT_SLAVE_STATE,sizeof(data_buff),data_buff,&p_data_send);  //打包好数据

    display_serial_port_data((char*)&p_data_send,data_len,SERIAL_PORT_TX);


}



void Network_Widget::on_pb_register_clicked()
{
    Reg_Dialog *reg_dialog = new Reg_Dialog();

    reg_dialog->exec();

    delete reg_dialog;
}

void Network_Widget::on_pb_clear_tx_clicked()
{
    welcome_ui->plainTextEdit_uart_tx->clear();
    welcome_ui->plainTextEdit_cardnum->clear();
    welcome_ui->plainTextEdit_doornum->clear();
    welcome_ui->plainTextEdit_state->clear();
}


void Network_Widget::on_pb_clear_rx_clicked()
{
    welcome_ui->plainTextEdit_uart_rx->clear();
    welcome_ui->plainTextEdit_cardnum->clear();
    welcome_ui->plainTextEdit_doornum->clear();
    welcome_ui->plainTextEdit_state->clear();
}













