#include "lamps_widget.h"

Lamps_Widget::Lamps_Widget(WelcomeWidget *widget)
{
    this->welcomewidget = widget;
    welcome_ui = this->welcomewidget->ui;



    connect(welcome_ui->pb_all_lamps_ctrl,SIGNAL(clicked()),this,SLOT(on_pb_all_lamps_ctrl_clicked()));     //建立信号槽
    connect(welcome_ui->pb_brightness_decrease,SIGNAL(clicked()),this,SLOT(on_pb_brightness_decrease_clicked()));     //建立信号槽
    connect(welcome_ui->pb_brightness_increase,SIGNAL(clicked()),this,SLOT(on_pb_brightness_increase_clicked()));     //建立信号槽
    connect(welcome_ui->horizontalSlider_brightness, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_brightness_valueChanged(int)));

    welcome_ui->label_brightness_value->setText(QString::number(welcome_ui->horizontalSlider_brightness->value(),10));
    welcome_ui->pb_all_lamps_ctrl->setText(trUtf8("全关"));
    welcome_ui->pb_all_lamps_ctrl->setCheckable(false);
}

Lamps_Widget::~Lamps_Widget()
{


}









void Lamps_Widget::on_pb_all_lamps_ctrl_clicked()
{
    static int current_brightness = -1;

    if(welcome_ui->pb_all_lamps_ctrl->isCheckable())
    {
        welcome_ui->horizontalSlider_brightness->setValue(current_brightness);

        welcome_ui->pb_all_lamps_ctrl->setText(trUtf8("全关"));
        welcome_ui->pb_all_lamps_ctrl->setCheckable(false);
    }
    else
    {
        current_brightness = welcome_ui->horizontalSlider_brightness->value();

        welcome_ui->horizontalSlider_brightness->setValue(0);

        welcome_ui->pb_all_lamps_ctrl->setText(trUtf8("全开"));
        welcome_ui->pb_all_lamps_ctrl->setCheckable(true);
    }
}


void Lamps_Widget::on_pb_brightness_decrease_clicked()
{
    welcome_ui->horizontalSlider_brightness->setValue(welcome_ui->horizontalSlider_brightness->value()-welcome_ui->horizontalSlider_brightness->singleStep());
}


void Lamps_Widget::on_pb_brightness_increase_clicked()
{
    welcome_ui->horizontalSlider_brightness->setValue(welcome_ui->horizontalSlider_brightness->value()+welcome_ui->horizontalSlider_brightness->singleStep());
}


void Lamps_Widget::on_horizontalSlider_brightness_valueChanged(int value)
{
    welcome_ui->label_brightness_value->setText(QString::number(value,10));
    welcome_ui->pb_all_lamps_ctrl->setText(trUtf8("全关"));
    welcome_ui->pb_all_lamps_ctrl->setCheckable(false);
}
