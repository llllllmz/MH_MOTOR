#pragma once
#include <QThread>
#include <QSerialPort>
#include "protocal.h"
class SendPack : public QThread
{
    Q_OBJECT
public:
    SendPack(QSerialPort *port);
    void run() override;

private:
    QSerialPort *serialPort;
    bool makePack(char cmd,char param,char data[],char *pack);
};

