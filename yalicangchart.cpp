#include "yalicangchart.h"
#include "ui_yalicangchart.h"
#include <QApplication>
#include <QMainWindow>
#include <QRandomGenerator>
#include <QDebug>
using namespace QtCharts;
yalicangChart::yalicangChart(int param,QWidget *parent) :
    QMainWindow(parent),
    param(param),
    ui(new Ui::yalicangChart)
{
    ui->setupUi(this);
    QChart *chart = new QChart();
    chart->setTitle("Real-time Pressure Chart");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // 创建折线图系列
    series = new QSplineSeries();
    series->setName("Pressure Data");

    // 添加系列到图表
    chart->addSeries(series);

    // 创建 X 轴（时间轴）
    axisX = new QDateTimeAxis();
    axisX->setFormat("hh:mm:ss"); // 设置时间格式
    axisX->setTitleText("Time");
    axisX->setTickCount(20); // 设置刻度数量
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // 创建 Y 轴
    axisY = new QValueAxis();
    axisY->setRange(800, 1200); // 设置压力范围
    axisY->setTitleText("Pressure (Pa)");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // 创建图表视图
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // 创建主窗口布局
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(chartView);
    setCentralWidget(centralWidget);
}

yalicangChart::~yalicangChart()
{
    delete ui;
}

void yalicangChart::slots_on_chart_ylc(char param, int data)
{
    if(param == this->param)
    {
        QDateTime currentTime = QDateTime::currentDateTime();

        // 如果数据点超过一定数量，删除最早的点
        if (series->count() > 20) {
            series->removePoints(0, 1); // 删除第一个点
        }

        // 添加新的数据点
        series->append(currentTime.toMSecsSinceEpoch(), data);




        // 更新 X 轴范围
        if (series->count() > 20) {
            QDateTime minTime = QDateTime::fromMSecsSinceEpoch(series->at(0).x());
            QDateTime maxTime = QDateTime::fromMSecsSinceEpoch(series->at(series->count() - 1).x());
            axisX->setRange(minTime, maxTime);
        } else {
            axisX->setRange(currentTime.addSecs(-10), currentTime);
        }

    }
}
