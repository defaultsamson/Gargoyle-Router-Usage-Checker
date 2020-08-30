#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include "mainwindow.h"
#include <QDialog>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <QPushButton>

namespace Ui {
class DialogSettings;
}

class DeletePushButton : public QPushButton
{
    Q_OBJECT

    int _profileIndex;
public:
    DeletePushButton(int profileIndex) : _profileIndex(profileIndex) {
        setText("");
        setIcon(QIcon(":/icons/res/delete.png"));
        setIconSize(QSize(24, 24));
        connect(this, &DeletePushButton::clicked, this, [&]{ emit deleteEntry(_profileIndex); });
    }

signals:
    void deleteEntry(int index);
};

class ProfileCheckBox : public QCheckBox
{
    Q_OBJECT

    int _profileIndex;
public:
    ProfileCheckBox(int profileIndex) : _profileIndex(profileIndex) {
        connect(this, &ProfileCheckBox::stateChanged, this, [&](int state){ emit checkEntry(_profileIndex, state == Qt::Checked); });
    }

signals:
    void checkEntry(int index, bool checked);
};

class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(MainWindow *main);
    ~DialogSettings();

protected:
    void resizeEvent(QResizeEvent *);
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *event);

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

    void on_checkBoxDarkTheme_stateChanged(int arg1);

    void on_horizontalSliderSnap_sliderMoved(int position);

    void on_spinBoxSnap_valueChanged(int arg1);

private:
    Ui::DialogSettings *ui;
    MainWindow *main;
    bool accepted = false;

    /// The index of the column containing checkboxes
    const int COL_CHECKBOX = 0;
    /// The index of the column containing the ip ranges
    const int COL_IP_RANGE = 1;
    /// The index of the column containing the names
    const int COL_NAME = 2;
    bool originalDarkTheme;

    QList<int> deletedIndexes;
    QList<int> checkedIndexes;
    void refreshTable(bool firstTime = false);
    void updateGridWidth();
};

#endif // DIALOGSETTINGS_H
