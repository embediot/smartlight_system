#ifndef REG_DIALOG_H
#define REG_DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>


namespace Ui {
class Reg_Dialog;
}


class Reg_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Reg_Dialog(QWidget *parent = 0);
    ~Reg_Dialog();



private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void read_register_serial_port();

private:
    Ui::Reg_Dialog *ui;

    unsigned int card_num;

};





#endif // REG_DIALOG_H
