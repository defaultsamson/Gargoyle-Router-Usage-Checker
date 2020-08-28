#include "dialogsettings.h"
#include "ui_dialogsettings.h"

#include "mainwindow.h"
#include "settings.h"

#include <QHeaderView>

DialogSettings::DialogSettings(MainWindow *main) :
    QDialog(main),
    main(main),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::Fixed);

    darkThemeOriginal = Settings::DARK_THEME.value().toBool();

    ui->checkBoxOtherWindows->setChecked(Settings::DISPLAY_ABOVE.value().toBool());
    ui->checkBoxSaveGeometry->setChecked(Settings::RELOAD_LOCATION.value().toBool());
    ui->checkBoxDarkTheme->setChecked(Settings::DARK_THEME.value().toBool());
    ui->checkBoxShowBar->setChecked(Settings::SHOW_QUOTA.value().toBool());
    ui->checkBoxShowGraph->setChecked(Settings::SHOW_GRAPH.value().toBool());
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
    main->setDarkTheme(darkThemeOriginal);
    close();
}

void DialogSettings::on_buttonBox_accepted()
{
    Settings::DISPLAY_ABOVE.setValue(ui->checkBoxOtherWindows->isChecked());
    Settings::RELOAD_LOCATION.setValue(ui->checkBoxSaveGeometry->isChecked());
    Settings::DARK_THEME.setValue(ui->checkBoxDarkTheme->isChecked());
    Settings::SHOW_QUOTA.setValue(ui->checkBoxShowBar->isChecked());
    Settings::SHOW_GRAPH.setValue(ui->checkBoxShowGraph->isChecked());
    close();
}

void DialogSettings::on_checkBoxDarkTheme_stateChanged(int state)
{
    main->setDarkTheme(state == Qt::CheckState::Checked);
}
