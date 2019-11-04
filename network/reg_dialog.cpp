#include "reg_dialog.h"
#include "ui_reg_dialog.h"


Reg_Dialog::Reg_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Reg_Dialog)
{
    ui->setupUi(this);


    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");    //将buttonbox中的ok 变成汉化
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

}

Reg_Dialog::~Reg_Dialog()
{
    delete ui;

}


void Reg_Dialog::on_buttonBox_accepted()
{
    QString str_temp;
    QString str_temp1 = "注册卡号";
    QString str_temp2 = "删除卡号";


    str_temp = ui->comboBox_operate->currentText();


    if(str_temp == str_temp1)
    {

    }
    else if(str_temp == str_temp2)
    {

    }
    else
    {

    }



}

void Reg_Dialog::on_buttonBox_rejected()
{
    qDebug() << "rejected button click";

}


void Reg_Dialog::read_register_serial_port()
{
    unsigned char i = 0;
    char card_num_ascii[8];
    char card_num_hex[4];
    int recv_len = 0;
    char data_buff[1024];

    if((data_buff[0] == 0x02) && (data_buff[1] == 0x30) && (data_buff[2] == 0x30))
    {
        memcpy(card_num_ascii,&data_buff[3],sizeof(card_num_ascii));

        for(i = 0;i < 8;i++)
        {
            if((card_num_ascii[i] > 0x40) && (card_num_ascii[i] < 0x47))
            {
                card_num_ascii[i] = card_num_ascii[i] - 0x37;
            }
            else if((card_num_ascii[i] >= 0x30) && (card_num_ascii[i] <= 0x39))
            {
                card_num_ascii[i] = card_num_ascii[i] - 0x30;
            }
        }

        memset(card_num_hex,0x00,sizeof(card_num_hex));

        card_num_hex[0] = (unsigned char)((card_num_ascii[0] << 4) | card_num_ascii[1]);
        card_num_hex[1] = (unsigned char)((card_num_ascii[2] << 4) | card_num_ascii[3]);
        card_num_hex[2] = (unsigned char)((card_num_ascii[4] << 4) | card_num_ascii[5]);
        card_num_hex[3] = (unsigned char)((card_num_ascii[6] << 4) | card_num_ascii[7]);

        card_num = (unsigned int)((card_num_hex[0] << 24) | (card_num_hex[1] << 16) |
                    (card_num_hex[2] << 8) | (card_num_hex[3] << 0));
    }

    ui->textBrowser_cardnum->setText(QString::number(card_num, 10));

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);

    for(int i = 0;i < 4;i++)
    {
        printf("0x%02x ",card_num_hex[i]);
    }
    printf("\n");

}
