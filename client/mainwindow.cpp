#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    UdpSetting = new UdpCnct;    
    connect(this, &MainWindow::signal, UdpSetting, &UdpCnct::SendData);
    connect(UdpSetting, &UdpCnct::signal, this, &MainWindow::EstablishConnect);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_UdpSetting_clicked()
{
    UdpSetting->show();
}

void MainWindow::EstablishConnect(const QList<QString> Datas)
{

    if (Datas[0] == NULL){
        serverAddress = "";
        serverPort = NULL;
        ui->connection_status->setText("Отключено");
    }
    else{
        serverAddress = Datas[0];
        serverPort = Datas[1].toUShort();
        ui->connection_status->setText("Подключено");

    }

}

QList<quint8> MainWindow::convertNumberToQuint8(uint16_t number) {
    QList<quint8> result;

    quint8 lowerByte = static_cast<quint8>(number & 0xFF); // Младший байт (низкие 8 бит)
    quint8 higherByte = static_cast<quint8>((number >> 8) & 0xFF); // Старший байт (высокие 8 бит)

    result.append(lowerByte);
    result.append(higherByte);

    return result;
}



QByteArray MainWindow::generatePacket(uint8_t command, QString str_number, bool b){
    DataPacket packet;
    if (b){
        uint16_t number = str_number.toUShort();
        QList<quint8> result = convertNumberToQuint8(number);

        packet.dataLSB = result[0]; // Младший байт данных
        packet.dataMSB = result[1]; // Старший байт данных
    }
    packet.command = command;

    uint16_t crcValue = calculateCRC16(reinterpret_cast<uint8_t*>(&packet), sizeof(packet) - 2);
    packet.crcLow = crcValue & 0xFF;
    packet.crcHigh = (crcValue >> 8) & 0xFF;
    return QByteArray(reinterpret_cast<const char*>(&packet), sizeof(DataPacket));

}

uint16_t MainWindow::calculateCRC16(const uint8_t *data, size_t length) {
    uint16_t crc = 0xFFFF; // Начальное значение CRC

    for (size_t i = 0; i < length; ++i) {
        crc ^= data[i]; // XOR с очередным байтом

        for (size_t j = 0; j < 8; ++j) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001; // Полином CRC-16 (0xA001)
            } else {
                crc = crc >> 1;
            }
        }
    }

    return crc;
}

QString MainWindow::byteArrayToString(const QByteArray& byteArray) {
    QString packetString;

    for (int i = 0; i < byteArray.size(); ++i) {
        packetString += QString("%1 ").arg(static_cast<unsigned char>(byteArray.at(i)), 2, 16, QChar('0')).toUpper();
    }

    return packetString.simplified().toUpper();
}

QByteArray MainWindow::wait_answer(){
    if (udpSocket.waitForReadyRead(100)) { // Ждем ответ в течение 100 мсек
       QByteArray responseData;
       responseData.resize(udpSocket.pendingDatagramSize());

       QHostAddress sender;
       quint16 senderPort;
       udpSocket.readDatagram(responseData.data(), responseData.size(), &sender, &senderPort);

       QString responseString = byteArrayToString(responseData);

       if (responseString == "Error"){
          ui->rec_status->setText("Ошибка");
       }else{
           ui->rec_status->setText(responseString);
       }

       return responseData;
   } else {

       ui->rec_status->setText("Нет ответа");
       return NULL;
   }
}

QByteArray MainWindow::writeBitsToQByteArray(quint8 number) {
    QByteArray byteArray(8, 0); // Создание массива байтов с нулевым значением
    QBitArray bitArray(8) ; // Создание массива битов

    for (int i = 0; i < 8; ++i) {
        bitArray.setBit(i, (number >> i) & 0x01); // Установка битов в QBitArray
    }

    // Копирование битов из QBitArray в QByteArray
    for (int i = 0; i < bitArray.size(); ++i) {
        byteArray[i] = bitArray[i];
    }

    return byteArray;
}

QByteArray MainWindow::SendToServer(uint8_t command, QString str_number, bool b){
    QByteArray datagram = generatePacket(command, str_number, b);
    QString packetString = byteArrayToString(datagram);
    ui->send_status->setText(packetString);
    udpSocket.writeDatagram(datagram, QHostAddress(serverAddress), serverPort);
    QByteArray response = wait_answer();
    return response;
}

void MainWindow::on_cmd1_on_clicked()
{
    SendToServer(0x01, 0, 0);
}


void MainWindow::on_cmd1_off_clicked()
{
    SendToServer(0x02, 0, 0);
}


void MainWindow::on_cmd2_on_clicked()
{
    SendToServer(0x03, 0, 0);
}


void MainWindow::on_cmd2_off_clicked()
{
    SendToServer(0x04, 0, 0);
}


void MainWindow::on_cmd3_on_clicked()
{
    SendToServer(0x05, 0, 0);
}


void MainWindow::on_cmd3_off_clicked()
{
    SendToServer(0x06, 0, 0);
}


void MainWindow::on_cmd4_on_clicked()
{
    SendToServer(0x07, 0, 0);
}


void MainWindow::on_cmd4_off_clicked()
{
    SendToServer(0x08, 0, 0);
}


void MainWindow::on_cmd5_on_clicked()
{
    SendToServer(0x09, 0, 0);
}


void MainWindow::on_cmd5_off_clicked()
{
    SendToServer(0x0a, 0, 0);
}


void MainWindow::on_sync_dynamic_clicked()
{
    SendToServer(0x0b, 0, 0);
}


void MainWindow::on_sync_static_clicked()
{
    SendToServer(0x0c, 0, 0);
}


void MainWindow::on_sync_rgb_clicked()
{
    SendToServer(0x0d, 0, 0);
}


void MainWindow::on_sync_BandW_clicked()
{
    SendToServer(0x0e, 0, 0);
}


void MainWindow::on_sync_1024str_clicked()
{
    SendToServer(0x0f, 0, 0);
}


void MainWindow::on_sync_576str_clicked()
{
    SendToServer(0x10, 0, 0);
}


void MainWindow::on_sync_progressive_clicked()
{
    SendToServer(0x11, 0, 0);
}


void MainWindow::on_sync_interlaced_clicked()
{
    SendToServer(0x12, 0, 0);
}


void MainWindow::on_identification_clicked()
{
    SendToServer(0x13, 0, 0);
}


void MainWindow::on_telemetry_clicked()
{
    QByteArray response = SendToServer(0x1e, 0, 0);

    QByteArray status1 = writeBitsToQByteArray(response[3]);
    QByteArray status2 = writeBitsToQByteArray(response[4]);

    if (status1.at(7) == 1) ui->lineEdit_2->setText("режим «576 строк»");
    else ui->lineEdit_2->setText("режим «1024 строк»");

    if (status1.at(6) == 1) ui->lineEdit_3->setText("«RGB» (цветной)режим");
    else ui->lineEdit_3->setText("«Чёрно-белый» режим");

    if (status1.at(5) == 1) ui->lineEdit_4->setText("«Динамическкий» режим");
    else ui->lineEdit_4->setText("«Статический» режим");

    if (status1.at(4) == 1) ui->lineEdit_5->setText("Вкл");
    else ui->lineEdit_5->setText("Выкл");

    if (status1.at(3) == 1) ui->lineEdit_6->setText("Вкл");
    else ui->lineEdit_6->setText("Выкл");

    if (status1.at(2) == 1) ui->lineEdit_7->setText("Вкл");
    else ui->lineEdit_7->setText("Выкл");

    if (status1.at(1) == 1) ui->lineEdit_8->setText("Вкл");
    else ui->lineEdit_8->setText("Выкл");

    if (status1.at(0) == 1) ui->lineEdit_9->setText("Вкл");
    else ui->lineEdit_9->setText("Выкл");

    if (status2.at(7) == 1) ui->lineEdit_10->setText("«Черезстрочная» развёртка");
    else ui->lineEdit_10->setText("«Прогрессивная» развёртка");

}






