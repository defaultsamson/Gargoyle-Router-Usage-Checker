#include "mainwindow.h"
#include "settings.h"
#include "updatethread.h"
#include <QThread>

UpdateThread::UpdateThread(GargoyleParser *parser, MainWindow *mainWindow)
{
    assert(parser);
    assert(mainWindow);

    this->parser = parser;
    this->mainWindow = mainWindow;

    connect(mainWindow, &MainWindow::updateProfiles, this, &UpdateThread::runUpdateLoop);
}

void UpdateThread::startUpdateLoop()
{
    loopActive = true;
    runUpdateLoop();
}

void UpdateThread::runUpdateLoop()
{
    timer.stop();

    qDebug("Updating usages...");
    if (parser->update(Settings::ROUTER_IP.value().toString(), mainWindow->profiles(), mainWindow->profileLock))
    {
        qDebug("Successfully updated usages");

        // Print profiles to console
        for (GargoyleProfile *profile : mainWindow->profiles())
        {
            if (profile->isUpdated())
            {
                Usage usage = profile->getUsage();
                qDebug("Range \"%s\": %llu bytes / %llu bytes at %lld bytes / %lld ns (%lld bytes/s)", qUtf8Printable(profile->name), usage.current, usage.max, profile->getUsageDelta(), profile->getTimeDelta().count(), profile->getUsagePerSecond());
            }
            else
            {
                qDebug("Range \"%s\": Not updated", qUtf8Printable(profile->name));
            }
        }

        emit afterUpdate();
    }
    else
    {
        qDebug("Failed to update usages");

        emit updateError();
    }

    if (loopActive)
        timer.start(Settings::UPDATE_SECONDS.value().toInt() * std::milli().den, this);
}

void UpdateThread::stopUpdateLoop()
{
    loopActive = false;
    timer.stop();
}

void UpdateThread::timerEvent(QTimerEvent *event)
{
    if (loopActive)
        runUpdateLoop();
}
