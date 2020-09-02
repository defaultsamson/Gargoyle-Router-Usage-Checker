#ifndef PROFILEBAR_H
#define PROFILEBAR_H

#include "updatethread.h"
#include "usagebar.h"

#include <QObject>

class MainWindow;

class ProfileUsageBar : public UsageBar
{
public:
    ProfileUsageBar(UpdateThread *updateThread, uint64_t profileKey, MainWindow *mainWindow);

    uint64_t getProfileKey() const;
    void setProfileKey(uint64_t profileKey);

private:
    uint64_t profileKey;
    MainWindow *mainWindow;

    GargoyleProfile *getDeviceProfile();

public slots:
    void updateUsageBar();
};

#endif // PROFILEBAR_H
