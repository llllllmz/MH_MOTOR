#pragma once

#include <QMainWindow>
#include <QSerialPort>
#include "sendpack.h"
#include "receivepack.h"
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void monitor();
    void updateData(char cmd,char param,int data);
    void updateTime();

signals:
    void on_chart_ylc(char param,int data);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serialPort;
    SendPack *senderThread;
    ReceivePack* receiverThread;
    QTimer* timer;

};
