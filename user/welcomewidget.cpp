#include "welcomewidget.h"
#include "ui_welcomewidget.h"

WelcomeWidget::WelcomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeWidget)
{
    ui->setupUi(this);

    ui->pb_lamps->click();

    watchdog = new WatchDogFeed(this);
    network_widget = new Network_Widget(this);
    setting_widget = new Setting_Widget(this);
    lamps_widget = new Lamps_Widget(this);
    qqlink_widget = new QQLink_Widget(this);
    camera_widget = new Camera_Widget(this);


    base_timer = new QTimer(this);
    connect(base_timer,SIGNAL(timeout()),this,SLOT(basetimer_timeout_handler()));
    base_timer->start(TIMEOUT_BASE_VALUE);

    update_information();
}





WelcomeWidget::~WelcomeWidget()
{
    delete watchdog;
    delete network_widget;
    delete setting_widget;
    delete lamps_widget;
    delete qqlink_widget;
    delete camera_widget;

    delete base_timer;

    delete ui;
}







//通过定时器不断刷新当前时间
void WelcomeWidget::update_current_time()
{
    QDateTime current_time = QDateTime::currentDateTime();
    QString str_time = current_time.toString("yyyy-MM-dd hh:mm:ss dddd");

    ui->label_time->setText(str_time);
}



//定时器超时处理
void WelcomeWidget::basetimer_timeout_handler()
{
    static unsigned int time_update_count = 0;

    ++time_update_count;

    if(time_update_count > (TIMEOUT_CURRTIME_UPDATE / TIMEOUT_BASE_VALUE-1))
    {
        time_update_count = 0;
        update_current_time();
    }

}
















void WelcomeWidget::on_pb_lamps_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->label_title->setText(ui->pb_lamps->text());
}

void WelcomeWidget::on_pb_camera_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->label_title->setText(ui->pb_camera->text());
}

void WelcomeWidget::on_pb_qqlink_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->label_title->setText(ui->pb_qqlink->text());
}

void WelcomeWidget::on_pb_network_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->label_title->setText(ui->pb_network->text());
}

void WelcomeWidget::on_pb_setting_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->label_title->setText(ui->pb_setting->text());
}


void WelcomeWidget::update_information()
{
    QString info;

    info = COMPANY_INFO + trUtf8("  软件版本 ") + SOFTWARE_VERSION \
            + trUtf8("  硬件版本 ") + HARDWARE_VERSION;

    ui->label_information->setText(info);
}








