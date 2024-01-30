#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <udpcnct.h>
#include <QUdpSocket>
#include <QList>
#include <QString>
#include <QHostAddress>
#include <QByteArray>
#include <QBitArray>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_UdpSetting_clicked();

    void on_cmd1_on_clicked();

    void on_cmd1_off_clicked();

    void on_cmd2_on_clicked();

    void on_cmd2_off_clicked();

    void on_cmd3_on_clicked();

    void on_cmd3_off_clicked();

    void on_cmd4_on_clicked();

    void on_cmd4_off_clicked();

    void on_cmd5_on_clicked();

    void on_cmd5_off_clicked();

    void on_sync_dynamic_clicked();

    void on_sync_static_clicked();

    void on_sync_rgb_clicked();

    void on_sync_BandW_clicked();

    void on_sync_1024str_clicked();

    void on_sync_576str_clicked();

    void on_sync_progressive_clicked();

    void on_sync_interlaced_clicked();

    void on_identification_clicked();

    void on_telemetry_clicked();



private:
    Ui::MainWindow *ui;
    UdpCnct *UdpSetting;

    QUdpSocket udpSocket;
    QString serverAddress = "127.0.0.1";
    quint16 serverPort = 12345;

    quint16 senderPort;
    QHostAddress sender;


    struct DataPacket {
        quint8 startByte = 0x14;
        quint8 address = 0x01;
        quint8 command;
        quint8 dataLSB = 0x00;
        quint8 dataMSB = 0x00;
        quint8 crcLow = 0x00;
        quint8 crcHigh = 0x00;
        quint8 stopByte = 0x88;
    };

    QByteArray generatePacket(uint8_t command, QString str_number = NULL, bool b = true);
    QList<quint8> convertNumberToQuint8(uint16_t number);
    uint16_t calculateCRC16(const uint8_t *data, size_t length);
    QString byteArrayToString(const QByteArray& byteArray);
    QByteArray wait_answer();
    QByteArray writeBitsToQByteArray(quint8 number);
    QByteArray SendToServer(uint8_t command, QString str_number, bool b);



signals:
    void signal();

public slots:
    void EstablishConnect(QList<QString> Datas);


};
#endif // MAINWINDOW_H
