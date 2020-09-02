#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gargoyleparser.h"
#include "gargoyleprofile.h"
#include "updatethread.h"
#include "usagegraph.h"
#include "usagebar.h"
#include "profileusagebar.h"

#include <QMainWindow>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QShowEvent>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setDarkTheme(bool set = true);
    void saveProfiles();
    void loadSettings(bool initial = false);

    QReadWriteLock profileLock;
    QMap<uint64_t, GargoyleProfile*> &profiles();

    void mousePress(QMouseEvent *event);
    void mouseMove(QMouseEvent *event);
    void mouseRelease(QMouseEvent *event);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

private:
    Ui::MainWindow *ui;
    UsageGraph *graph;
    ProfileUsageBar *usageBar;
    QList<ProfileUsageBar*> extendedUsageBars;

    static const QString JSON_PROFILES;
    static const QString JSON_IP_RANGE;
    static const QString JSON_NAME;
    static const QString JSON_ACTIVE;

    QPalette darkPalette;
    QPalette defaultPalette;

    /// Used for keeping track of dragging the window
    QPointF oldRelativePos;
    bool dragStarted = false;

    GargoyleParser parser;
    QMap<uint64_t, GargoyleProfile*> _profiles;

    QThread *updateThread;
    UpdateThread *updateThreadWorker;

private slots:
    void showContextMenu(const QPoint &pos);
    void openOptions();

signals:
    void updateProfiles();
};
#endif // MAINWINDOW_H
