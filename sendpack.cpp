#include "sendpack.h"
#include <QDebug>
SendPack::SendPack(QSerialPort *port)
{
    serialPort = port;
}

void SendPack::run()
{
    unsigned char cmdAndParam[][2] = {
        {ZHU_JI_WEN_DU_REQ,0},
        {ZHOU_SU_DU_REQ,1},
        {ZHOU_SU_DU_REQ,2},
        {YA_LI_CHANG_REQ,1},
        {YA_LI_CHANG_REQ,2},
        {YA_LI_CHANG_REQ,3},
        {YA_LI_CHANG_REQ,4},
        {YA_LI_CHANG_REQ,5},
        {GANG_YOU_LIANG_REQ,1},
        {GANG_YOU_LIANG_REQ,2},
        {GANG_YOU_LIANG_REQ,3},
        {JI_XIE_BI_REQ,0},
        {DONG_LI_GAN_SU_DU_SET_REQ,0},
    };

    char pack[9];
    char data[4] = {0};
    int cmdCount = sizeof(cmdAndParam)/sizeof(cmdAndParam[0]);
    while(1)
    {
        for(int i=0;i<cmdCount;i++)
        {
            makePack(cmdAndParam[i][0],cmdAndParam[i][1],data,pack);
            int ret = serialPort->write(pack,sizeof(pack));
            qDebug() <<"send:"<<ret<<"bytes.";
            serialPort->waitForBytesWritten(500);
        }
        QThread::msleep(3000);
    }
}

bool SendPack::makePack(char cmd, char param, char data[], char *pack)
{
    if(!pack) return false;

    pack[0] = 0xEF;
    pack[1] = cmd;
    pack[2] = param;

    pack[3] = data[0];
    pack[4] = data[1];
    pack[5] = data[2];
    pack[6] = data[3];

    int count = 0;
    for(int k=1;k<=6;k++)
    {
        char byte = pack[k];

        for(int i=0;i<8;i++)
        {
            if(byte&1)count++;
            byte >>=1;
        }
    }

    if(count&1)
    {
        pack[7]=1;
    }else
    {
        pack[7]=0;
    }
    pack[8]=0xFE;
    return true;
}
