#include "update_dialog.h"
#include "ui_update_dialog.h"

Update_Dialog::Update_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Update_Dialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确 定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取 消");

    ui->radioButton_usb_update->setChecked(true);

}

Update_Dialog::~Update_Dialog()
{

    delete ui;
}





void Update_Dialog::on_buttonBox_accepted()
{
    if(ui->radioButton_usb_update->isChecked())
    {
        qDebug() << "radioButton_usb_update is checked";
        start_update_process(USB_UPGRADE);
    }
    else if(ui->radioButton_network_update->isChecked())
    {
        qDebug() << "radioButton_network_update is checked";
        start_update_process(NET_UPGRADE);
    }
    else
    {

    }
}

void Update_Dialog::on_buttonBox_rejected()
{
    qDebug() << "rejected button click";

}

bool Update_Dialog::start_update_process(unsigned char flag)
{
    WatchDogFeed watchdog;

    if(flag == USB_UPGRADE)
    {
        if(QFile::exists("/run/media/sda4/"))
        {
            if(QFile::exists("/run/media/sda4/system_imx6q/system.ini"))
            {
                watchdog.onUpgradeAPP(USB_UPGRADE);
            }
            else
            {
                QMessageBox::warning(this, "WARNNING", tr("<font color='white'>请检查配置文件！</font>"));
            }
        }
        else
        {
            QMessageBox::warning(this, "WARNNING", tr("<font color='white'>请检查配置文件！</font>"));

        }
    }
    else if(flag == NET_UPGRADE)
    {
        watchdog.onUpgradeAPP(NET_UPGRADE);
    }
    else
    {}

    return true;
}


