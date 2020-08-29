#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdlib.h>

#include <QMenu>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QShowEvent>
#include <QScreen>

#include "dialogsettings.h"
#include "settings.h"
#include "gargoyleparser.h"
#include "gargoyleprofile.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , defaultPalette(qApp->palette())
{
    ui->setupUi(this);

    // Sets window as borderless
    // setWindowFlag();
    // Enables custom right click menu
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showContextMenu(const QPoint &)));

    // Colours from https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle/blob/master/DarkStyle.cpp
    // (mirror): https://stackoverflow.com/a/45634644/1902411
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::Base, QColor(42, 42, 42));
    darkPalette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::Dark, QColor(35, 35, 35));
    darkPalette.setColor(QPalette::Shadow, QColor(20, 20, 20));
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80, 80, 80));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(127, 127, 127));

    loadSettings();
}

void MainWindow::loadSettings() {
    setDarkTheme(Settings::DARK_THEME.value().toBool());

    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::FramelessWindowHint;
    if (Settings::DISPLAY_ABOVE.value().toBool())
        flags |= Qt::WindowStaysOnTopHint;
    else
        flags &= ~Qt::WindowStaysOnTopHint;

    setWindowFlags(flags);
    show();
}

void MainWindow::setDarkTheme(bool set) {
    qApp->setPalette(set ? darkPalette : defaultPalette);
    // Settings::DARK_THEME.setValue(set);
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
    if (evt->button() == Qt::LeftButton) {
        dragStarted = false;

        const int thresh = Settings::SNAP_THRESHOLD.value().toInt();

        // Snap to screen edge if there's a given pixel threshold
        if (thresh > 0) {
            // The current screen
            QScreen* s = screen();
            QRect g = s->availableGeometry();

            // Screen geometry
            const int sLeft = g.x();
            const int sTop = g.y();
            const int sRight = sLeft + g.width();
            const int sBottom = sTop + g.height();

            // Window geometry
            const int wLeft = x();
            const int wTop = y();
            const int wRight = wLeft + width();
            const int wBottom = wTop + height();

            // Window destination x and y
            int x = wLeft, y = wTop;

            if (abs(wLeft - sLeft) < thresh) {
                // Snap to left
                x = sLeft;
            } else if (abs(sRight - wRight) < thresh) {
                // Snap to right
                x = sRight - width();
            }

            if (abs(wTop - sTop) < thresh) {
                // Snap to top
                y = sTop;
            } else if (abs(sBottom - wBottom) < thresh) {
                // Snap to bottom
                y = sBottom - height();
            }

            setGeometry(x, y, width(), height());
        }
    }
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
    parser.update(Settings::ROUTER_IP.value().toString(), _profiles);
}

void MainWindow::openOptions() {
    DialogSettings s(this);
    s.exec();
}

const QList<GargoyleProfile*> &MainWindow::profiles() {
    return _profiles;
}

MainWindow::~MainWindow()
{
    delete ui;

    delete Settings::QSETTINGS;
}

