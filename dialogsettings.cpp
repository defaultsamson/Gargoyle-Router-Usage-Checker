#include "dialogsettings.h"
#include "ui_dialogsettings.h"

#include "mainwindow.h"
#include "settings.h"

#include <QHeaderView>
#include <QTableWidgetItem>
#include <algorithm>

DialogSettings::DialogSettings(MainWindow *main) :
    QDialog(main),
    ui(new Ui::DialogSettings),
    main(main)
{
    ui->setupUi(this);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(COL_CHECKBOX, QHeaderView::ResizeMode::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(COL_IP_RANGE, QHeaderView::ResizeMode::Fixed);

    originalDarkTheme = Settings::DARK_THEME.value().toBool();

    ui->checkBoxOtherWindows->setChecked(Settings::DISPLAY_ABOVE.value().toBool());
    ui->checkBoxSaveGeometry->setChecked(Settings::RELOAD_LOCATION.value().toBool());
    ui->checkBoxDarkTheme->setChecked(Settings::DARK_THEME.value().toBool());
    ui->checkBoxShowBar->setChecked(Settings::SHOW_QUOTA.value().toBool());
    ui->checkBoxShowGraph->setChecked(Settings::SHOW_GRAPH.value().toBool());

    const int MAX_SNAP_DISTANCE = 500;
    ui->horizontalSliderSnap->setRange(0, MAX_SNAP_DISTANCE);
    ui->horizontalSliderSnap->setValue(Settings::SNAP_THRESHOLD.value().toInt());
    ui->spinBoxSnap->setRange(0, MAX_SNAP_DISTANCE);
    ui->spinBoxSnap->setValue(Settings::SNAP_THRESHOLD.value().toInt());
    ui->spinBoxSeconds->setValue(Settings::UPDATE_SECONDS.value().toInt());
    ui->spinBoxSeconds->setRange(1, 60 * 60); // From 1 second to 1 hour between updates
    ui->lineEditIP->setText(Settings::ROUTER_IP.value().toString());

    // Test data
    QTableWidget *t = ui->tableWidget;

    for (int i = 0; i < main->profiles().size(); ++i) {
        GargoyleProfile *profile = main->profiles().at(i);

        t->insertRow(i);
        QCheckBox *checkBox = new QCheckBox();
        checkBox->setChecked(profile->showInGraph);
        checkboxes.append(checkBox);

        QHBoxLayout *layout = new QHBoxLayout();
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(checkBox);
        layout->setAlignment(Qt::AlignCenter);

        QWidget *widget = new QWidget();
        widget->setContentsMargins(0, 0, 0, 0);
        widget->setLayout(layout);

        t->setCellWidget(i, COL_CHECKBOX, widget);
        t->setCellWidget(i, COL_IP_RANGE, new QLabel(profile->displayIpRange));
        t->setItem(i, COL_NAME, new QTableWidgetItem(profile->name));
    }
}

/// Manually updates the width of the columns in the grid, because resizing the columns is broken
void DialogSettings::updateGridWidth() {
    const int BUTTON_COLUMN = 20;
    const double RANGE_MAX_SIZE = 230;
    int remainingWidth = ui->tableWidget->width() - BUTTON_COLUMN;
    ui->tableWidget->horizontalHeader()->resizeSection(0, BUTTON_COLUMN);
    ui->tableWidget->horizontalHeader()->resizeSection(1, std::min(remainingWidth * 0.6, RANGE_MAX_SIZE)); // Set IP Range to 60% of the remaining area
    // Just set this size to 1, because the table is set to automatically fill out the final column
    ui->tableWidget->horizontalHeader()->resizeSection(2, 1);
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
    accepted = false;
    close();
}

void DialogSettings::closeEvent(QCloseEvent *event) {
    if (!accepted) main->setDarkTheme(originalDarkTheme);
}

void DialogSettings::on_buttonBox_accepted()
{
    accepted = true;

    Settings::DISPLAY_ABOVE.setValue(ui->checkBoxOtherWindows->isChecked());
    Settings::RELOAD_LOCATION.setValue(ui->checkBoxSaveGeometry->isChecked());
    Settings::DARK_THEME.setValue(ui->checkBoxDarkTheme->isChecked());
    Settings::SHOW_QUOTA.setValue(ui->checkBoxShowBar->isChecked());
    Settings::SHOW_GRAPH.setValue(ui->checkBoxShowGraph->isChecked());

    Settings::SNAP_THRESHOLD.setValue(ui->spinBoxSnap->value());
    Settings::UPDATE_SECONDS.setValue(ui->spinBoxSeconds->value());
    Settings::ROUTER_IP.setValue(ui->lineEditIP->text());

    for (int i = 0; i < checkboxes.size() && i < main->profiles().size(); ++i) {
        main->profiles().at(i)->name = ui->tableWidget->item(i, COL_NAME)->text();
        main->profiles().at(i)->showInGraph = checkboxes.at(i)->isChecked();
    }

    main->saveProfiles();
    main->loadSettings();

    close();
}

void DialogSettings::on_checkBoxDarkTheme_stateChanged(int state)
{
    // Temporarily change dark theme
    main->setDarkTheme(state == Qt::CheckState::Checked);
}

void DialogSettings::on_horizontalSliderSnap_sliderMoved(int value)
{
    ui->spinBoxSnap->setValue(value);
}

void DialogSettings::on_spinBoxSnap_valueChanged(int value)
{
    ui->horizontalSliderSnap->setValue(value);
}
