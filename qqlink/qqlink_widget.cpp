#include "qqlink_widget.h"

QQLink_Widget::QQLink_Widget(WelcomeWidget *widget)
{
    this->welcomewidget = widget;
    welcome_ui = this->welcomewidget->ui;


    welcome_ui->pb_link_qq_server->setEnabled(true);
    welcome_ui->pb_link_qq_server->setText(trUtf8("连接QQ"));
    welcome_ui->pb_link_qq_server->setCheckable(true);

    connect(welcome_ui->pb_link_qq_server,SIGNAL(clicked()),this,SLOT(on_pb_link_qq_server_clicked()));
}

QQLink_Widget::~QQLink_Widget()
{

}



void QQLink_Widget::on_pb_link_qq_server_clicked()
{

    if(welcome_ui->pb_link_qq_server->isCheckable())
    { 
        welcome_ui->label_link_status->setText(trUtf8("连接成功！"));

        welcome_ui->pb_link_qq_server->setText(trUtf8("断开QQ"));
        welcome_ui->pb_link_qq_server->setCheckable(false);
        welcome_ui->pb_link_qq_server->setEnabled(false);

    }
    else
    {
        welcome_ui->label_link_status->clear();

        welcome_ui->pb_link_qq_server->setText(trUtf8("连接QQ"));
        welcome_ui->pb_link_qq_server->setCheckable(true);
    }
}







