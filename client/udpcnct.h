#ifndef UDPCNCT_H
#define UDPCNCT_H

#include <QWidget>
#include <QUdpSocket>

namespace Ui {
class UdpCnct;
}

class UdpCnct : public QWidget
{
    Q_OBJECT

public:
    explicit UdpCnct(QWidget *parent = nullptr);
    ~UdpCnct();

private slots:
    void on_Close_clicked();

    void on_connct_clicked();

    void on_discnct_clicked();

public slots:
    void SendData();

private:
    Ui::UdpCnct *ui;

    QUdpSocket *udpSocket;

signals:
    void signal(QList<QString> Datas);
};

#endif // UDPCNCT_H
