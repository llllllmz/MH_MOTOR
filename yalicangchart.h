#pragma once
#include <QSplineSeries>
#include <mainwindow.h>
#include <QChart>
#include <QChartView>
#include <QSplineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QVBoxLayout>
#include <QWidget>
#include <QDateTime>
namespace Ui {
class yalicangChart;
}
using namespace QtCharts;
class yalicangChart : public QMainWindow
{
    Q_OBJECT

public:
    explicit yalicangChart(int param,QWidget *parent = nullptr);
    ~yalicangChart();

public:
    void slots_on_chart_ylc(char param,int data);

private:
    Ui::yalicangChart *ui;
    int param;
    QSplineSeries *series;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;
    QChart *chart;
};

