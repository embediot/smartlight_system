#ifndef UPDATE_DIALOG_H
#define UPDATE_DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QFile>
#include <QProcess>
#include <QPushButton>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include "user/watchdogfeed.h"
#include "network_reply_timeout.h"

namespace Ui {
class Update_Dialog;
}







class Update_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Update_Dialog(QWidget *parent = 0);
    ~Update_Dialog();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();    

private:
    Ui::Update_Dialog *ui;

    bool start_update_process(unsigned char flag);
};

#endif // UPDATE_DIALOG_H
