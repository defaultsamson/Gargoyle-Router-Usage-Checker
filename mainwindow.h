#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gargoyleparser.h"
#include "gargoyleprofile.h"
#include "updatethread.h"

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

    QMap<uint64_t, GargoyleProfile*> &profiles();

protected:
    void mousePressEvent(QMouseEvent *evt);
    void mouseMoveEvent(QMouseEvent *evt);
    void mouseReleaseEvent(QMouseEvent *event);

    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

private:
    Ui::MainWindow *ui;
    static const QString JSON_PROFILES;
    static const QString JSON_IP_RANGE;
    static const QString JSON_NAME;
    static const QString JSON_ACTIVE;

    QPalette darkPalette;
    QPalette defaultPalette;

    /// Used for keeping track of dragging the window
    QPoint oldRelativePos;
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
