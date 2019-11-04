#include "setting/setting_widget.h"



Setting_Widget::Setting_Widget(WelcomeWidget *widget)
{
    this->welcomewidget = widget;
    welcome_ui = this->welcomewidget->ui;

    connect(welcome_ui->pb_update,SIGNAL(clicked()),this,SLOT(on_pb_update_clicked()));     //建立信号槽
    connect(welcome_ui->pb_time_update,SIGNAL(clicked()),this,SLOT(on_pb_time_update_clicked()));     //建立信号槽
}

Setting_Widget::~Setting_Widget()
{


}



void Setting_Widget::on_pb_update_clicked()
{
    //qDebug() << "on_pb_update_clicked()";

    Update_Dialog *update_dialog = new Update_Dialog();

    update_dialog->exec();

    delete update_dialog;
}


void Setting_Widget::on_pb_time_update_clicked()
{
    //welcome_ui->label_time->setText(trUtf8("正在进行时间同步..."));

    QProcess proc;

    proc.execute("ntpdate 202.112.29.82");

    if(proc.waitForFinished(30000))
    {
        qDebug() << "time update timeout";
        proc.close();

        QMessageBox::information(new Update_Dialog(), "INFORMATION", tr("<font color='white'>连接服务器超时</font>"));
    }
    else
    {
        if(proc.exitStatus() == QProcess::CrashExit)
        {
            qDebug() << "time update fail";
            proc.close();

            QMessageBox::information(new Update_Dialog(), "INFORMATION", tr("<font color='white'>时间同步失败</font>"));
        }
        else
        {
            qDebug() << "time update success";
            proc.start("hwclock --systohc");

            QMessageBox::information(new Update_Dialog(), "INFORMATION", tr("<font color='white'>时间同步成功</font>"));


            if(proc.waitForFinished())
            {
                proc.close();
            }
            else
            {
                if(proc.exitStatus() == QProcess::CrashExit)
                {
                    proc.close();
                }
                else
                {
                }
            }
        }
    }

    proc.close();

//    connect(&proc,SIGNAL(finished(int, QProcess::ExitStatus)),this,SLOT(slot_time_update_finished(int, QProcess::ExitStatus)));     //建立信号槽
}


void Setting_Widget::slot_time_update_finished(int exit_code, QProcess::ExitStatus exit_status)
{
    qDebug() << exit_code << exit_status;

    //QMessageBox::information(this, "INFORMATION", tr("<font color='white'>时间同步成功</font>"));

    QProcess proc;

    proc.startDetached("hwclock --systohc");

}
