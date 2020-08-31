#ifndef UPDATETHREAD_H
#define UPDATETHREAD_H

#include "gargoyleparser.h"

#include <QBasicTimer>
#include <QObject>

class MainWindow;

class UpdateThread : public QObject
{
    Q_OBJECT
public:
    UpdateThread(GargoyleParser *parser, MainWindow *mainWindow);

private:
    GargoyleParser *parser;
    MainWindow *mainWindow;

    bool loopActive = false;
    QBasicTimer timer;

    void timerEvent(QTimerEvent *event) override;

public slots:
    void startUpdateLoop();
    void runUpdateLoop();
    void stopUpdateLoop();
};
#endif // UPDATETHREAD_H
