#include "dialogsettings.h"
#include "ui_dialogsettings.h"

#include <QHeaderView>

DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::Fixed);
}

/// Manually updates the width of the columns in the grid, because resizing the columns is broken
void DialogSettings::updateGridWidth() {
    int maxSize = ui->tableWidget->width();
    ui->tableWidget->horizontalHeader()->resizeSection(0, maxSize/2);
    ui->tableWidget->horizontalHeader()->resizeSection(1, maxSize/2);
}

void DialogSettings::resizeEvent(QResizeEvent *) {
    updateGridWidth();
}

void DialogSettings::showEvent(QShowEvent *) {
    updateGridWidth();
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::on_buttonBox_rejected()
{
    // TODO undo temp applied settings like dark mode
    close();
}

void DialogSettings::on_buttonBox_accepted()
{
    // TODO save settings
    close();
}
