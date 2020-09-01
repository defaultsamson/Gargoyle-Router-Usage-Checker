#include "usagegraph.h"

#include "mainwindow.h"

#include <QChartView>

UsageGraph::UsageGraph(MainWindow *main) : main(main)
{
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setBackgroundRoundness(0);

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
    //series->poin
    //series->setPen(QPen(QColor(255, 100, 100)));
    chart->addSeries(series);
    //series->attachAxis(axisX);
    //series->attachAxis(axisY);

    this->setContentsMargins(0, 0, 0, 0);
    this->setRenderHint(QPainter::Antialiasing);
    this->setChart(chart);
}

void UsageGraph::mousePressEvent(QMouseEvent *event) {
    main->mousePress(event);
}
void UsageGraph::mouseMoveEvent(QMouseEvent *event) {
    main->mouseMove(event);
}
void UsageGraph::mouseReleaseEvent(QMouseEvent *event) {
    main->mouseRelease(event);
}
