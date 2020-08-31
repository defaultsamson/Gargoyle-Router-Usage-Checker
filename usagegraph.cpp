#include "usagegraph.h"

#include <QChartView>

UsageGraph::UsageGraph(QWidget *)
{
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->setMargins(QMargins(0, 0, 0, 0));

    chart->createDefaultAxes();

    /*
    QValueAxis *axisX = new QValueAxis();
    axisX->setTickCount(20);
    chart->addAxis(axisX, Qt::AlignBottom);
    axisX->s

    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    */



    QSplineSeries *series = new QSplineSeries();
    *series << QPointF(1, 5) << QPointF(3.5, 18) << QPointF(4.8, 7.5) << QPointF(10, 2.5);
    //series->setPen(QPen(QColor(255, 100, 100)));
    chart->addSeries(series);
    //series->attachAxis(axisX);
    //series->attachAxis(axisY);

    this->setContentsMargins(0, 0, 0, 0);

    this->setChart(chart);
}
