#ifndef USAGEGRAPH_H
#define USAGEGRAPH_H

#include <QtCharts>
using namespace QtCharts;

class UsageGraph : public QChartView
{
public:
    UsageGraph(QWidget *parent = nullptr);
};

#endif // USAGEGRAPH_H
