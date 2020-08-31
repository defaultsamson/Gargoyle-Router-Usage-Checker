#ifndef USAGEBAR_H
#define USAGEBAR_H

#include <QProgressBar>
#include <QLabel>

class UsageBar : public QProgressBar
{
public:
    UsageBar();

    void paintEvent(QPaintEvent *);

    void setText(QString text);
    QString text();

private:
    QLabel *_text;
};

#endif // USAGEBAR_H
