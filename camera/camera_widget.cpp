#include "camera_widget.h"



Camera_Widget::Camera_Widget(WelcomeWidget *widget)
{
    bool open_camera_success = false;

    this->welcomewidget = widget;
    welcome_ui = this->welcomewidget->ui;

    connect(welcome_ui->pb_open_camera,SIGNAL(clicked()),this,SLOT(on_pb_open_camera_clicked()));
    connect(welcome_ui->pb_take_photos,SIGNAL(clicked()),this,SLOT(on_pb_take_photos_clicked()));
    connect(welcome_ui->pb_take_videos,SIGNAL(clicked()),this,SLOT(on_pb_take_videos_clicked()));

    //open_camera_success = cam_operate->open_camera();

#ifndef HARDWARE_ENCDEC
    if(open_camera_success)
    {
        welcome_ui->pb_open_camera->setText(trUtf8("打开摄像头"));
        welcome_ui->pb_open_camera->setCheckable(true);
        welcome_ui->pb_open_camera->setEnabled(true);
    }
    else
    {       
        welcome_ui->label_camera->setText(trUtf8("请连接摄像头"));
        welcome_ui->pb_open_camera->setCheckable(true);
        welcome_ui->pb_open_camera->setEnabled(false);
    }

#else


#endif

}

Camera_Widget::~Camera_Widget()
{
#ifndef HARDWARE_ENCDEC

#else

#endif

}


void Camera_Widget::on_pb_open_camera_clicked()
{
    //qDebug() << "on_pb_open_camera_clicked()";

    if(welcome_ui->pb_open_camera->isCheckable())
    {
            #ifndef HARDWARE_ENCDEC

            #else

            #endif
            welcome_ui->pb_open_camera->setText(trUtf8("关闭摄像头"));
            welcome_ui->pb_open_camera->setCheckable(false);
    }
    else
    {
        #ifndef HARDWARE_ENCDEC

        #else

        #endif

        welcome_ui->label_camera->clear();
        welcome_ui->pb_open_camera->setText(trUtf8("打开摄像头"));
        welcome_ui->pb_open_camera->setCheckable(true);
    }
}


void Camera_Widget::on_pb_take_photos_clicked()
{

}

void Camera_Widget::on_pb_take_videos_clicked()
{

}


void Camera_Widget::slot_show_pixmap(QPixmap pixmap)
{
    //qDebug() << ">>>>>>>>> Camera_Widget::slot_show_pixmap >>>>>>>>>>";

    QMutex mutex;

    mutex.lock();
    welcome_ui->label_camera->setPixmap(pixmap);
    mutex.unlock();
}




