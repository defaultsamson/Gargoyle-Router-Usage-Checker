#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *evt);
    void mouseMoveEvent(QMouseEvent *evt);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;

    /// Used for keeping track of dragging the window
    QPoint oldRelativePos;
    bool dragStarted = false;

private slots:
    void showContextMenu(const QPoint &pos);
    void updateData();
    void openOptions();
};
#endif // MAINWINDOW_H
