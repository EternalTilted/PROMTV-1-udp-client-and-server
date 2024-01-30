#include <QCoreApplication>
#include <QtNetwork>
#include <iostream>


class UDPServer : public QObject {
    Q_OBJECT

public:
    UDPServer() {
        udpSocket = new QUdpSocket(this);
        udpSocket->bind(QHostAddress::Any, 12345); // Здесь 12345 - порт для прослушивания

        connect(udpSocket, &QUdpSocket::readyRead, this, &UDPServer::processPendingDatagrams);
        std::cout << "UDP Server started. Listening for incoming connections on port 12345..." << std::endl;

    }

public slots:
    void processPendingDatagrams() {
        while (udpSocket->hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(udpSocket->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;

            udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
            std::cout << "Received datagram from " << sender.toString().toStdString() << ":" << senderPort << std::endl;
            /*
            if(datagram.at(0) == 'h' and datagram.at(1) == 'e'){
                udpSocket->writeDatagram(datagram, sender, senderPort);
            }
*/
            DataPacket packet;

            packet.startByte = datagram[0];
            packet.address = datagram[1];
            packet.command = datagram[2];
            packet.dataLSB = datagram[3];
            packet.dataMSB = datagram[4];
            packet.crcLow = 0x00;
            packet.crcHigh = 0x00;
            packet.stopByte = 0x88;
            uint16_t crcValue = calculateCRC16(reinterpret_cast<uint8_t*>(&packet), sizeof(packet) - 2);
            packet.crcLow = crcValue & 0xFF;
            packet.crcHigh = (crcValue >> 8) & 0xFF;


            if(packet.crcLow == static_cast<quint8>(datagram[5]) and packet.crcHigh == static_cast<quint8>(datagram[6])){

                if (static_cast<quint8>(datagram.at(2)) == 0x01){
                    udpSocket->writeDatagram(datagram, sender, senderPort);
                    cmd1 = 0x01;
                    status1 = replaceBit(status1, 0, 1);
                }

                if (static_cast<quint8>(datagram.at(2)) == 0x02) {
                    udpSocket->writeDatagram(datagram, sender, senderPort);
                    cmd1 = 0x02;
                    status1 = replaceBit(status1, 0, 0);
                }

                if (static_cast<quint8>(datagram.at(2)) == 0x03)  {
                    udpSocket->writeDatagram(datagram, sender, senderPort);
                    cmd2 = 0x03;
                    status1 = replaceBit(status1, 1, 1);
                }

                if (static_cast<quint8>(datagram.at(2)) == 0x04)  {
                    udpSocket->writeDatagram(datagram, sender, senderPort);
                    cmd2 = 0x04;
                    status1 = replaceBit(status1, 1, 0);
                }

                if (static_cast<quint8>(datagram.at(2)) == 0x05) {
                    udpSocket->writeDatagram(datagram, sender, senderPort);
                    cmd3 = 0x05;
                    status1 = replaceBit(status1, 2, 1);
                }

                if (static_cast<quint8>(datagram.at(2)) == 0x06){
                    udpSocket->writeDatagram(datagram, sender, senderPort);
                    cmd3 = 0x06;
                    status1 = replaceBit(status1, 2, 0);
                }

                if (static_cast<quint8>(datagram.at(2)) == 0x07) {
                    udpSocket->writeDatagram(datagram, sender, senderPort);
                    cmd4 = 0x07;
                    status1 = replaceBit(status1, 3, 1);
                }

                if (static_cast<quint8>(datagram.at(2)) == 0x08) {
                    udpSocket->writeDatagram(datagram, sender, senderPort);
                    cmd4 = 0x08;
                    status1 = replaceBit(status1, 3, 0);
                }

                if (static_cast<quint8>(datagram.at(2)) == 0x09) {
                    udpSocket->writeDatagram(datagram, sender, senderPort);
                    cmd5 = 0x09;
                    status1 = replaceBit(status1, 4, 1);
                }
                if (static_cast<quint8>(datagram.at(2)) == 0x0a) {
                    udpSocket->writeDatagram(datagram, sender, senderPort);
                    cmd5 = 0x0a;
                    status1 = replaceBit(status1, 4, 0);
                }

                if (static_cast<quint8>(datagram.at(2)) == 0x0b) {
                    udpSocket->writeDatagram(datagram, sender, senderPort);
                    dinamic = 0x0b;
                    status1 = replaceBit(status1, 5, 1);
                }

                if (static_cast<quint8>(datagram.at(2)) == 0x0c){
                    udpSocket->writeDatagram(datagram, sender, senderPort);
                    dinamic = 0x0c;
                    status1 = replaceBit(status1, 5, 0);
                }

                if (static_cast<quint8>(datagram.at(2)) == 0x0d) {
                    udpSocket->writeDatagram(datagram, sender, senderPort);
                    color = 0x0d;
                    status1 = replaceBit(status1, 6, 1);
                }

                if (static_cast<quint8>(datagram.at(2)) == 0x0e){
                    udpSocket->writeDatagram(datagram, sender, senderPort);
                    color = 0x0e;
                    status1 = replaceBit(status1, 6, 0);
                }

                if (static_cast<quint8>(datagram.at(2)) == 0x0f){
                    udpSocket->writeDatagram(datagram, sender, senderPort);
                    str = 0x0f;
                    status1 = replaceBit(status1, 7, 0);
                }

                if (static_cast<quint8>(datagram.at(2)) == 0x10) {
                    udpSocket->writeDatagram(datagram, sender, senderPort);
                    str = 0x10;
                    status1 = replaceBit(status1, 7, 1);
                }

                if (static_cast<quint8>(datagram.at(2)) == 0x11) {
                    udpSocket->writeDatagram(datagram, sender, senderPort);
                    interlaced = 0x11;
                    status2 = replaceBit(status1, 7, 0);
                }

                if (static_cast<quint8>(datagram.at(2)) == 0x12)  {
                    udpSocket->writeDatagram(datagram, sender, senderPort);
                    interlaced = 0x12;
                    status2 = replaceBit(status1, 7, 1);
                }

                if(static_cast<quint8>(datagram.at(2)) == 0x13){
                    //не понял, что и как нужно возвращать(
                    udpSocket->writeDatagram(datagram, sender, senderPort);
                }

                if (static_cast<quint8>(datagram.at(2)) == 0x1e){
                    datagram[3] = status1;
                    datagram[4] = status2;
                    udpSocket->writeDatagram(datagram, sender, senderPort);

                }
            }else {
                QByteArray datagram = "Error";
                udpSocket->writeDatagram(datagram, sender, senderPort);
            }

         }



    }


private:
    QUdpSocket *udpSocket;

    uint8_t status1 = 0b10000000;
    uint8_t status2 = 0b10000000;

    uint8_t cmd1 = 0x02;
    uint8_t cmd2 = 0x04;
    uint8_t cmd3 = 0x06;
    uint8_t cmd4 = 0x08;
    uint8_t cmd5 = 0x0a;
    uint8_t dinamic = 0x0c;
    uint8_t color = 0x0e;
    uint8_t str = 0x10;
    uint8_t interlaced = 0x12;

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

    uint8_t replaceBit(uint8_t number, uint8_t position, bool newValue) {
        // Маска для установки или сброса бита
        uint8_t mask = 1 << position;

        if (newValue) {
            // Установить бит в позиции
            number |= mask;
        } else {
            // Сбросить бит в позиции
            number &= ~mask;
        }

        return number;
    }



    uint16_t calculateCRC16(const uint8_t *data, size_t length) {
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

};

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    UDPServer server;

    return a.exec();
}

#include "main.moc"

