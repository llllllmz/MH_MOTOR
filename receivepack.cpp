#include "receivepack.h"
#include <QDebug>
ReceivePack::ReceivePack(QSerialPort *port)
{
    serialPort = port;
}

void ReceivePack::run()
{
    unsigned char pack[9];
    char cmd;
    char param;
    int data;

    while(1)
    {
        serialPort->waitForReadyRead(10);
        int ret = serialPort->bytesAvailable();
        if(ret == 0) continue;

        ret = serialPort->read((char *)pack,sizeof(pack));

        qDebug()<<"read "<<ret << " bytes:"<<
                  QString::number(pack[0],16) << QString::number(pack[1],16) << QString::number(pack[2],16)
                    << QString::number(pack[3],16) << QString::number(pack[4],16)
                    << QString::number(pack[5],16) << QString::number(pack[6],16)
                    << QString::number(pack[7],16) << QString::number(pack[8],16);

        if(parsePack((unsigned char*)pack,sizeof(pack),&cmd,&param,&data))
        {
            qDebug()<<"parse succeed!";
            emit receiveResponsePack(cmd,param,data);
        }else
        {
            qDebug()<<"收到非法包!";
        }
    }
}

bool ReceivePack::parsePack(unsigned char pack[], int size, char *cmd, char *param, int *data)
{
    if(!pack||size<9)return false;

    if(pack[0] == 0xEF&&pack[8]==0xFE)
    {
        int count = 0;
        for(int i=1;i<=7;i++)
        {
            char tmp = pack[i];
            for(int k=0;k<8;k++)
            {
                if(tmp &1)count++;
                tmp>>=1;
            }
        }

        if(count % 2)
        {
            return false;
        }
        *cmd = pack[1];
        *param = pack[2];
        *data = pack[3] | (pack[4]<<8)|(pack[5]<<16)|(pack[6]<<24)|(pack[7]<<32);
        qDebug()<<"data:"<<data;
        return true;
    }
    return false;
}
