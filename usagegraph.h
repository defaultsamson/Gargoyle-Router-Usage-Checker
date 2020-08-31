#ifndef USAGEGRAPH_H
#define USAGEGRAPH_H

class MainWindow; // Foward declaration

#include <QtCharts>
using namespace QtCharts;

class UsageGraph : public QChartView
{
public:
    UsageGraph(MainWindow *main);

protected:
    MainWindow *main;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // USAGEGRAPH_H
