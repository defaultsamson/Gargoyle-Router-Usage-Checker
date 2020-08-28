#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMenu>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QShowEvent>

#include "dialogsettings.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Sets window as borderless
    setWindowFlag(Qt::FramelessWindowHint);
    // Enables custom right click menu
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showContextMenu(const QPoint &)));
}

/// Records the initial oldRelativePos upon dragging
void MainWindow::mousePressEvent(QMouseEvent *evt) {
    if (evt->button() == Qt::LeftButton) {
        oldRelativePos = evt->pos();
        dragStarted = true;
    }
}

/// Moves the window to the mouse pos subtracted by the oldRelativePos
void MainWindow::mouseMoveEvent(QMouseEvent *evt) {
    if (dragStarted) move(evt->globalPos().x() - oldRelativePos.x(), evt->globalPos().y() - oldRelativePos.y());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *evt) {
    if (evt->button() == Qt::LeftButton) dragStarted = false;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Save the _main window sizes
    Settings::PREV_GEOMETRY.setValue(saveGeometry());
}

void MainWindow::showEvent(QShowEvent *event) {
    // Reloads previous window location, if set to do so
    if (Settings::RELOAD_LOCATION.value().toBool() &&  Settings::PREV_GEOMETRY.hasValue())
        restoreGeometry(Settings::PREV_GEOMETRY.value().toByteArray());
}

/// Shows the right click menu.
void MainWindow::showContextMenu(const QPoint &pos) {
    QMenu contextMenu(tr("Context menu"), this);

    QAction action1("Update", this);
    connect(&action1, SIGNAL(triggered()), this, SLOT(updateData()));
    contextMenu.addAction(&action1);

    QAction action2("Settings", this);
    connect(&action2, SIGNAL(triggered()), this, SLOT(openOptions()));
    contextMenu.addAction(&action2);

    QAction action3("Close", this);
    connect(&action3, SIGNAL(triggered()), this, SLOT(close()));
    contextMenu.addAction(&action3);

    contextMenu.exec(mapToGlobal(pos));
}

void MainWindow::updateData() {

}

void MainWindow::openOptions() {
    DialogSettings s;
    s.exec();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete Settings::QSETTINGS;
}

