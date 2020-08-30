#include "dialogsettings.h"
#include "ui_dialogsettings.h"

#include "mainwindow.h"
#include "settings.h"

#include <QHeaderView>
#include <QTableWidgetItem>
#include <algorithm>
#include <QPushButton>
#include <iostream>

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

    refreshTable(true);
}

void DialogSettings::refreshTable(bool firstTime) {
    QTableWidget *t = ui->tableWidget;
    // Clear the table
    for (int i = t->rowCount() - 1; i >= 0; i--) t->removeRow(i);

    //QList<uint64_t> rangeKeys = main->profiles().keys();
    //for (int i = 0; i < rangeKeys.size(); ++i) {
    //    GargoyleProfile *profile = main->profiles()[rangeKeys[i]];
    int tableI = 0;
    for (int i = 0; i < main->profiles().size(); ++i) {

        // If the profile index has been queued for deletion, skip it and maintain the tableI
        if (deletedIndexes.contains(i)) continue;

        GargoyleProfile *profile = main->profiles().at(i);

        t->insertRow(tableI);
        ProfileCheckBox *checkBox = new ProfileCheckBox(i);
        checkBox->setChecked(firstTime ? profile->showInGraph : checkedIndexes.contains(i));
        // If it's being initialized, add the checkedIndex the first time
        if (firstTime && profile->showInGraph) checkedIndexes.append(i);

        QHBoxLayout *layout = new QHBoxLayout();
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setAlignment(Qt::AlignCenter);

        QWidget *widget = new QWidget();
        widget->setContentsMargins(0, 0, 0, 0);
        widget->setLayout(layout);

        QObject::connect(checkBox, &ProfileCheckBox::checkEntry, this, [&](int index, bool checked){
            if (checked)
                this->checkedIndexes.append(index);
            else
                this->checkedIndexes.removeAll(index);
        });

        QLabel *label = new QLabel(profile->displayIpRange);

        QHBoxLayout *layout2 = new QHBoxLayout();
        layout2->setContentsMargins(0, 0, 0, 0);
        layout2->setAlignment(Qt::AlignCenter);
        layout2->addWidget(label);

        QWidget *widget2 = new QWidget();
        widget2->setContentsMargins(0, 0, 0, 0);
        widget2->setLayout(layout2);


        if (profile->updated) {
            layout->addWidget(checkBox);
        } else {
            DeletePushButton *but = new DeletePushButton(i);
            layout->addWidget(but);

            QObject::connect(but, &DeletePushButton::deleteEntry, this, [&](int index){
                this->deletedIndexes.append(index);
                this->refreshTable();
            });
        }

        t->setCellWidget(tableI, COL_CHECKBOX, widget);
        t->setCellWidget(tableI, COL_IP_RANGE, widget2);
        t->setItem(tableI, COL_NAME, new QTableWidgetItem(profile->name));

        tableI++;
    }
}

/// Manually updates the width of the columns in the grid, because resizing the columns is broken
void DialogSettings::updateGridWidth() {
    const int BUTTON_COLUMN = 20;
    const double RANGE_MAX_SIZE = 230;
    int remainingWidth = ui->tableWidget->width() - BUTTON_COLUMN;
    ui->tableWidget->horizontalHeader()->resizeSection(COL_CHECKBOX, BUTTON_COLUMN);
    ui->tableWidget->horizontalHeader()->resizeSection(COL_IP_RANGE, std::min(remainingWidth * 0.6, RANGE_MAX_SIZE)); // Set IP Range to 60% of the remaining area
    // Just set this size to 1, because the table is set to automatically fill out the final column
    ui->tableWidget->horizontalHeader()->resizeSection(COL_NAME, 1);
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

    //QList<uint64_t> rangeKeys = main->profiles().keys();
    //for (int i = 0; i < checkboxes.size() && i < rangeKeys.size(); ++i) {
    //    main->profiles()[rangeKeys[i]]->name = ui->tableWidget->item(i, COL_NAME)->text();
    //    main->profiles()[rangeKeys[i]]->showInGraph = checkboxes.at(i)->isChecked();
    // Set all as false
    for (int i = 0; i < main->profiles().size(); ++i) main->profiles().at(i)->showInGraph = false;
    // Then, set true based on the checked indexes
    for (int i = 0; i < checkedIndexes.size(); ++i) {
        int index = checkedIndexes.at(i);
        main->profiles().at(index)->showInGraph = true;
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
