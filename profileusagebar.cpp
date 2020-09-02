#include "profileusagebar.h"
#include "mainwindow.h"
#include "mathhelper.h"

ProfileUsageBar::ProfileUsageBar(UpdateThread *updateThread, uint64_t profileKey, MainWindow *mainWindow) : UsageBar()
{
    this->profileKey = profileKey;
    this->mainWindow = mainWindow;

    connect(updateThread, &UpdateThread::afterUpdate, this, &ProfileUsageBar::updateUsageBar);
    updateUsageBar();
}

uint64_t ProfileUsageBar::getProfileKey() const
{
    return profileKey;
}

void ProfileUsageBar::setProfileKey(uint64_t profileKey)
{
    this->profileKey = profileKey;
    updateUsageBar();
}

GargoyleProfile *ProfileUsageBar::getDeviceProfile()
{
    mainWindow->profileLock.lockForRead();
    for (GargoyleProfile *profile : mainWindow->profiles())
    {
        if (profile->deviceProfile && profile->isUpdated())
        {
            mainWindow->profileLock.unlock();
            return profile;
        }
    }
    mainWindow->profileLock.unlock();
    return nullptr;
}

void ProfileUsageBar::updateUsageBar()
{
    mainWindow->profileLock.lockForRead();
    // If the profile key isn't 0, get the associated profile, otherwise, use the device profile
    GargoyleProfile *profile = profileKey > 0 ? mainWindow->profiles().value(profileKey, nullptr) : getDeviceProfile();
    if (profile != nullptr && profile->isUpdated())
    {
        Usage usage = profile->getUsage();
        int32_t usagePercentInt = (usage.current * 100) / usage.max;

        // Clamp the percentage to reasonable values
        setValue(std::max(0, std::min(100, usagePercentInt)));

        // Now set progress bar display
        /*
         * KB = B >> 10
         * MB = KB >> 10 = B >> 20
         * GB = MB >> 10 = KB >> 20 = B >> 30
         */

        // double usagePercentDouble = static_cast<double>(usage.current * 100.0) / static_cast<double>(usage.max);
        // usagePercentDouble = std::round(usagePercentDouble * 100.0) / 100.0; // Precise to 2 decimal places
        double usagePercentDouble = MathHelper::percentage(usage.current, usage.max);

        // Current usage in gb
        double currentUsage = MathHelper::decimalPoint(MathHelper::ratio(usage.current, 1073741824)); // 1073741824 == 2^30 (bytes to gb)
        // Max usage in gb
        double maxUsage = MathHelper::decimalPoint(MathHelper::ratio(usage.max, 1073741824));
        // Download speed in megabits per second
        double speed = MathHelper::decimalPoint(MathHelper::ratio(profile->getUsagePerSecond(), 131072)); // 131072 == (bytes to megabytes) 2^20 / 2^3 (bytes to bits)

        setText(QString::number(usagePercentDouble) + "% (" + QString::number(currentUsage) + "/" + QString::number(maxUsage) + "GB) (" + QString::number(speed) + "Mb/s" + ")");

        qDebug("Updated profile bar for key %llu", profileKey);
    }
    else
    {
        reset();
        qDebug("Unable to find profile for key %llu", profileKey);
    }
    mainWindow->profileLock.unlock();
}
