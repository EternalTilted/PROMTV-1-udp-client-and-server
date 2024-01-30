#include "udpcnct.h"
#include "ui_udpcnct.h"


UdpCnct::UdpCnct(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UdpCnct)
{
    ui->setupUi(this);



}

UdpCnct::~UdpCnct()
{
    delete ui;
}

void UdpCnct::on_Close_clicked()
{
    this->close();
}


void UdpCnct::on_connct_clicked()
{
    SendData();
}

void UdpCnct::SendData()
{
    QList<QString> Datas;
    Datas.push_back(ui->IP->text());
    Datas.push_back(ui->PortSnd->text());
    emit signal(Datas);

}

void UdpCnct::on_discnct_clicked()
{
    QList<QString> Datas{NULL};
    emit signal(Datas);
}

