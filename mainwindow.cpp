#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "protocal.h"
#include <QDateTime>
#include <QDebug>
#include "yalicangchart.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer;
    timer->setInterval(1000);
    timer->setSingleShot(false);
    timer->setTimerType(Qt::PreciseTimer);
    timer->start();
    connect(timer,&QTimer::timeout,this,&MainWindow::updateTime);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::monitor()
{
    serialPort = new QSerialPort;
    serialPort->setPortName("COM2");
    if(!serialPort->open(QIODevice::ReadWrite))
    {
        QMessageBox::critical(nullptr,"串口打开失败",
                              "串口COM2不存在或者已被占用！",
                              QMessageBox::Ok);
        QTimer::singleShot(0,qApp,SLOT(quit()));
    }
    serialPort->setBaudRate(19200);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    senderThread = new SendPack(serialPort);
    receiverThread = new ReceivePack(serialPort);

    senderThread->start();
    receiverThread->start();

    connect(receiverThread,&ReceivePack::receiveResponsePack,
            this,&MainWindow::updateData);
}

void MainWindow::updateData(char cmd, char param, int data)
{
    switch(cmd)
    {
        case ZHU_JI_WEN_DU_RSP:
            ui->label_temperature->setText(QString::number(data));
            // 判断温度是否超过 100 度
                    if (data > 100)
                    {
                        // 将字体颜色设置为红色
                        ui->label_temperature->setStyleSheet("color: red;");
                    }
                    else
                    {
                        // 如果温度不超过 100 度，恢复默认颜色（假设默认颜色是黑色）
                        ui->label_temperature->setStyleSheet("color: white;");
                    }
        break;
    case ZHOU_SU_DU_RSP:
        if(param == 1)
        {
            ui->label_zhu_zhou->setNum(data);
        }else if(param == 2)
        {
            ui->label_fu_zhou->setNum(data);
        }
        break;
    case GANG_YOU_LIANG_RSP:
        if(param == 1) ui->label_yl_1->setText(QString::asprintf("%d L",data));
        else if(param == 2) ui->label_yl_2->setText(QString::asprintf("%d L",data));
        else if(param == 3) ui->label_yl_3->setText(QString::asprintf("%d L",data));
        break;
    case YA_LI_CHANG_RSP:
        if (param == 1) {
            ui->lb_yl_1->setText(QString::asprintf("%d Pa", data));
            ui->pb_cang_1->setValue(data);
            if (data > 1100) {
                ui->pb_cang_1->setStyleSheet("QProgressBar::chunk { background-color: red; }");
                qWarning() << "overload yalicang" << param << data;
            } else {
                ui->pb_cang_1->setStyleSheet("QProgressBar::chunk { background-color:rgb(0,185,84); }"); // 恢复默认颜色
            }
            emit on_chart_ylc(1,data);
        } else if (param == 2) {
            ui->lb_yl_2->setText(QString::asprintf("%d Pa", data));
            ui->pb_cang_2->setValue(data);
            if (data > 1100) {
                ui->pb_cang_2->setStyleSheet("QProgressBar::chunk { background-color: red; }");
                qWarning() << "overload yalicang" << param << data;
            } else {
                ui->pb_cang_2->setStyleSheet("QProgressBar::chunk { background-color:rgb(0,185,84); }"); // 恢复默认颜色
            }
        } else if (param == 3) {
            ui->lb_yl_3->setText(QString::asprintf("%d Pa", data));
            ui->pb_cang_3->setValue(data);
            if (data > 1100) {
                ui->pb_cang_3->setStyleSheet("QProgressBar::chunk { background-color: red; }");
                qWarning() << "overload yalicang" << param << data;
            } else {
                ui->pb_cang_3->setStyleSheet("QProgressBar::chunk { background-color:rgb(0,185,84); }"); // 恢复默认颜色
            }
        } else if (param == 4) {
            ui->lb_yl_4->setText(QString::asprintf("%d Pa", data));
            ui->pb_cang_4->setValue(data);
            if (data > 1100) {
                ui->pb_cang_4->setStyleSheet("QProgressBar::chunk { background-color: red; }");
                qWarning() << "overload yalicang" << param << data;
            } else {
                ui->pb_cang_4->setStyleSheet("QProgressBar::chunk {background-color:rgb(0,185,84); }"); // 恢复默认颜色
            }
        } else if (param == 5) {
            ui->lb_yl_5->setText(QString::asprintf("%d Pa", data));
            ui->pb_cang_5->setValue(data);
            if (data > 1100) {
                ui->pb_cang_5->setStyleSheet("QProgressBar::chunk { background-color: red; }");
                qWarning() << "overload yalicang" << param << data;
            } else {
                ui->pb_cang_5->setStyleSheet("QProgressBar::chunk { background-color:rgb(0,185,84); }"); // 恢复默认颜色
            }
        }
        break;
    case JI_XIE_BI_RSP:
        ui->lb_jxb->setText(QString::asprintf("%d L",data));
        ui->pb_jxb_6->setValue(data);
        break;
    }
}

void MainWindow::updateTime()
{
    ui->label_time->setText(QDateTime::currentDateTime().toString());
}




void MainWindow::on_pushButton_clicked()
{
    yalicangChart* y =new yalicangChart(1,this);
    connect(this,&MainWindow::on_chart_ylc,y,&yalicangChart::slots_on_chart_ylc);
    y->show();
}
