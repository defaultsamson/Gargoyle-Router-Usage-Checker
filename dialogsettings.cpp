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

    int tableI = 0;
    QList<uint64_t> rangeKeys = main->profiles().keys();
    for (int i = 0; i < rangeKeys.size(); ++i) {
        uint64_t range = rangeKeys[i];
        GargoyleProfile *profile = main->profiles()[range];

        if (profileChanges.contains(range)) {
            // If the profile index has been queued for deletion, skip  from the table (and maintain the tableI)
            if (profileChanges[range].deleted)
                continue;
        } else {
            // Create a Changes object if there isn't one
            profileChanges[range] = Changes { false, profile->showInGraph, profile->name };
        }

        Changes changes = profileChanges[range];

        // 1. Create row
        t->insertRow(tableI);

        // 2.1 Create first column layout
        QHBoxLayout *layout = new QHBoxLayout();
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setAlignment(Qt::AlignCenter);

        QWidget *widget = new QWidget();
        widget->setContentsMargins(0, 0, 0, 0);
        widget->setLayout(layout);

        // 2.2 Choose whether to put a delete button or a checkbox
        if (profile->isUpdated()) {
            ProfileCheckBox *checkBox = new ProfileCheckBox(range);
            checkBox->setChecked(changes.checked);
            layout->addWidget(checkBox);

            QObject::connect(checkBox, &ProfileCheckBox::checkEntry, this, [&](uint64_t range, bool checked){
                this->profileChanges[range].checked = checked;
            });
        } else {
            DeletePushButton *but = new DeletePushButton(range);
            layout->addWidget(but);

            QObject::connect(but, &DeletePushButton::deleteEntry, this, [&](uint64_t range){
                this->profileChanges[range].deleted = true;
                this->refreshTable();
            });
        }

        // 3. Create IP Range
        QLabel *label = new QLabel(profile->displayIpRange);

        QHBoxLayout *layout2 = new QHBoxLayout();
        layout2->setContentsMargins(0, 0, 0, 0);
        layout2->setAlignment(Qt::AlignCenter);
        layout2->addWidget(label);

        QWidget *widget2 = new QWidget();
        widget2->setContentsMargins(0, 0, 0, 0);
        widget2->setLayout(layout2);

        // 4. Create Name Edit
        ProfileTextItem *textInput = new ProfileTextItem(range, profileChanges[range].name);

        QObject::connect(textInput, &ProfileTextItem::textChanged, this, [&](uint64_t range, QString text){
            this->profileChanges[range].name = text;
        });

        QHBoxLayout *layout3 = new QHBoxLayout();
        layout3->setContentsMargins(0, 0, 0, 0);
        layout3->setAlignment(Qt::AlignCenter);
        layout3->addWidget(textInput);

        QWidget *widget3 = new QWidget();
        widget3->setContentsMargins(0, 0, 0, 0);
        widget3->setLayout(layout3);

        // 5. Add all the widgets to the row
        t->setCellWidget(tableI, COL_CHECKBOX, widget);
        t->setCellWidget(tableI, COL_IP_RANGE, widget2);
        t->setCellWidget(tableI, COL_NAME, widget3);

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

    QList<uint64_t> rangeKeys = profileChanges.keys();
    for (int i = 0; i < rangeKeys.size(); i++) {
        uint64_t range = rangeKeys[i];
        if (main->profiles().contains(range) && profileChanges.contains(range)) {
            GargoyleProfile *profile = main->profiles()[range];
            Changes changes = profileChanges[range];
            if (changes.deleted) {
                main->profiles().remove(range);
            } else {
                profile->showInGraph = changes.checked;
                profile->name = changes.name;
            }
        }
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
